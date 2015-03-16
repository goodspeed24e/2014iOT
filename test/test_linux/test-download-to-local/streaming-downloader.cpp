#include "streaming-downloader.hpp"
#include <sstream>
#include <openssl/md5.h>
#include <cstdio>
#include <cstdlib>
#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>


#define SIZE_OF_INPUT_BUFFER (1024*1024)
#define SIZE_OF_OUTPUT_BUFFER (MD5_DIGEST_LENGTH + 1)

typedef bool ThreadResult;

struct ThreadFuture
{
	boost::shared_ptr<boost::thread> job_thread;
	boost::shared_future<ThreadResult> job_result_future;
};

typedef std::deque<ThreadFuture> ThreadContainer;

/// The queue to store the thread.
static ThreadContainer THREAD_QUEUE;

/**
 * @brief Constructor of StreamingDownloader.
 *
 * @param[in] s_info The information for streaming client.
 * @param[in] end_time The end time of iavf file for downloading.
 * @param[in] output_path The output path for downloading files (excluding the filename).
 * @param[in] output_filename The prefix of output filename.
 * @param[in] split_file_size The max size of each split file.
 * @param[in] time_zone The time zone.
 * @param[in] callback The callback function for downloading status.
 * @param[in] progress_callback The callback function fro downloading progress.
 */
StreamingDownloader::StreamingDownloader(const instek::StreamingClient::StreamingInfo& s_info,
		unsigned int end_time,
		const std::string& iavf_output_path,
		const std::string& idb_output_path,
		const std::string& output_filename,
		size_t split_file_size,
		int time_zone,
		const DownloadStatusCallback& callback,
		const DownloadProgressCallback& progress_callback)
	: iavf_writer_(NULL),
	streaming_info_(s_info),
	end_time_(end_time),
	split_file_size_(split_file_size),
	time_zone_(time_zone),
	file_index_(1),
	callback_(callback),
	progress_callback_(progress_callback),
	total_packet_size_in_list_(0)
{
	// Create path (If the path contains filename, it will throw exception boost::system::error_code)
#ifndef DO_NOT_AUTO_CREATE_IAVF_OUTPUT_PATH
	boost::filesystem::create_directories(iavf_output_path);
#endif
	boost::filesystem::create_directories(idb_output_path);

	// Generate the filepath
#ifndef WIN32
	char separator = '/';
#else
	char separator = '\\';
#endif
	iavf_output_filepath_ = iavf_output_path;
	if(iavf_output_path[iavf_output_path.size() -1] != separator)
		iavf_output_filepath_ += separator;
	iavf_output_filepath_ += output_filename;

	idb_output_filepath_ = idb_output_path;
	if(idb_output_path[idb_output_path.size() -1] != separator)
		idb_output_filepath_ += separator;
	idb_output_filepath_ += output_filename;

#ifndef ONLY_DOWNLOAD
	// Create new files for iavf and idb file.
	if(!this->create_new_files())
	{
		if(callback_)
			callback_(-1);
		return;
	}
#endif

	// Create streaming client and start it.
	streaming_client_.reset(new instek::StreamingClient(s_info));
	if(!streaming_client_)
	{
		fprintf(stderr, "Can not create streaming client.\n");
		if(callback_)
			callback_(-1);
		return;
	}

	streaming_client_->SetCallbackFunc(boost::bind(&StreamingDownloader::handle_packet, this, _1, _2, _3));
	//streaming_client_->SetReconnectTime(5);
	streaming_client_->SetTimeoutValue(10);
	streaming_client_->SetErrorCallbackFunc(boost::bind(&StreamingDownloader::streaming_err_callback_func, this, _1));
	streaming_client_->Start();
}

/// Destructor of StreamingDownloader.
StreamingDownloader::~StreamingDownloader()
{
	if(streaming_client_)
	{
		streaming_client_->Stop();
	}

	// Join each post-processing thread.
	ThreadContainer::iterator itor = THREAD_QUEUE.begin();
	for(; itor != THREAD_QUEUE.end(); ++itor)
	{
		itor->job_thread->join();
		itor->job_thread.reset();
	}
	THREAD_QUEUE.clear();

	if (iavf_writer_)
		delete iavf_writer_;
}

/**
 * @brief Callback function for streaming client when error occurs.
 *
 * @param[in] err_code The error code.
 */
void StreamingDownloader::streaming_err_callback_func(int /*err_code*/)
{
	fprintf(stderr, "network error................\n");

	// Tell the status for the client which using this streaming downloader.
	if(callback_)
		callback_(-2);
}

/**
 * @brief The function to create new iavf and idb file and increate the index of file.
 *
 * @return true: Success, false: Failed.
 */
bool StreamingDownloader::create_new_files()
{
	std::stringstream str_stream;
	str_stream << "-" << file_index_;

	current_iavf_filepath_ = iavf_output_filepath_ + str_stream.str();
	current_iavf_filepath_ += ".ivf";

	current_idb_filepath_ = idb_output_filepath_ + str_stream.str();
	current_idb_filepath_ += ".idb";

	iavf_writer_ = new iavfng::CAVFileWriter;
	if(!iavf_writer_)
	{
		fprintf(stderr, "Can not create iavf writer.\n");
		return false;
	}

	// Create the iavf file (If file does not exist, the file is created. Otherwise, it will be truncated.)
	boost::filesystem::ofstream temp_outfile(current_iavf_filepath_, std::ios_base::out | std::ios_base::trunc);
	if(temp_outfile.fail())
	{
		fprintf(stderr, "Can not create iavf file: %s\n", current_iavf_filepath_.c_str());
		temp_outfile.close();
		return false;
	}
	temp_outfile.close();

	if(iavf_writer_->Open(current_iavf_filepath_, current_idb_filepath_, streaming_info_.camera_id, iavfng::CAVFileWriter::CREATE, split_file_size_) != instek::SlotRetValue::OK)
	{
		fprintf(stderr, "Can not create iavf file and database.\n");
		return false;
	}

	++file_index_;
	return true;
}

/**
 * @brief Function to check the result of post-process.
 *
 * @return true: Success. false: Failed.
 */
bool StreamingDownloader::is_success_in_post_process()
{
	ThreadContainer::iterator itor = THREAD_QUEUE.begin();
	for(; itor != THREAD_QUEUE.end(); ++itor)
	{
		itor->job_result_future.wait();

		if(! (itor->job_result_future.valid()))
			return false;

		if(! (itor->job_result_future.is_ready()) )
			return false;

		if(! (itor->job_result_future.has_value()) )
			return false;

		if(itor->job_result_future.has_exception())
			return false;

		if(itor->job_result_future.get() == false)
			return false;
	}

	return true;
}

/**
 * @brief Create thread to do post-process.
 *
 * @return true: always return true.
 */
bool StreamingDownloader::post_process()
{
	boost::packaged_task<ThreadResult> p_task(boost::bind(&StreamingDownloader::complete_iavf, this, current_iavf_filepath_, current_idb_filepath_, iavf_writer_));

	ThreadFuture temp_thread_future;
	temp_thread_future.job_result_future = p_task.get_future();
	temp_thread_future.job_thread.reset(new boost::thread(boost::move(p_task)));
	iavf_writer_ = NULL;
	THREAD_QUEUE.push_back(temp_thread_future);

	return true;
}

/**
 * @brief Function to do the post-process for iavf file.
 *
 * @param[in] iavf_path The filepath of iavf file.
 * @param[in] idb_path The filepath of idb file.
 * @param[in] writer The iavf writer.
 * @return true: Success, false: Failed.
 */
bool StreamingDownloader::complete_iavf(const string& iavf_path, const string& idb_path, iavfng::CAVFileWriter* writer)
{
	// Wait for the main thread to create file first.
	::sleep(1);
	// Flush files.
	unsigned int timestamp = 0;
	if(!writer || (writer->Flush(timestamp, 2) != instek::SlotRetValue::OK)) // flush & update full
	{
		//log there
		fprintf(stderr, "Can not flush files.\n");
	}
	delete writer;
	writer = NULL;

	boost::system::error_code err;
	if( !boost::filesystem::exists(iavf_path, err) )
	{
		fprintf(stderr, "File %s does not exist.\n", iavf_path.c_str());
		return false;
	}

	if(err)
	{
		fprintf(stderr, "Test exist error, file %s. err msg: %s\n", iavf_path.c_str(), err.message().c_str());
		return false;
	}

	boost::filesystem::fstream iavf_file(iavf_path, std::ios_base::in | std::ios_base::out | std::ios_base::binary);
	if(iavf_file.fail())
	{
		fprintf(stderr, "Can not open file %s.\n", iavf_path.c_str());
		return false;
	}

	// Write the offset of idb database in the iavf file.
	if(!write_idb_offset(iavf_file))
	{
		iavf_file.close();
		return false;
	}

	// Generate the MD5 checksum.
	unsigned char out_buffer[SIZE_OF_OUTPUT_BUFFER] = {'\0'};
	if(!generate_checksum(iavf_file, out_buffer))
	{
		iavf_file.close();
		return false;
	}
	// Write the time zone information into the idb file.
	iavfng::MetadataAccessor meta_accessor;
	meta_accessor.Open(idb_path, iavfng::MetadataAccessor::APPEND_DB);
	meta_accessor.SetIntegerProperty("time_zone", time_zone_);
	// Transfer the MD5 checksum to string.
	char out_buffer2[(MD5_DIGEST_LENGTH << 1) + 1] = {'\0'};
	char* buf_ptr = out_buffer2;
	for(int i = 0; i < MD5_DIGEST_LENGTH; ++i)
	{
		sprintf(buf_ptr, "%02x", out_buffer[i]);
		buf_ptr = buf_ptr + 2;
	}
	// Write the MD5 information into the idb file.
	meta_accessor.SetStringProperty("watermark", out_buffer2);
	meta_accessor.Close();

	// Append the idb file to iavf file.
	boost::filesystem::fstream idb_file(idb_path, std::ios_base::in | std::ios_base::binary);
	if(idb_file.fail())
	{
		fprintf(stderr, "Can not open file %s.\n", idb_path.c_str());
		iavf_file.close();
		return false;
	}
	if(!append_idb_to_iavf(iavf_file, idb_file))
	{
		fprintf(stderr, "Can not append idb file to iavf file.\n");
		iavf_file.close();
		idb_file.close();
		return false;
	}

	iavf_file.close();
	idb_file.close();

	// Remove the idb file.
	if( !boost::filesystem::remove(idb_path, err) )
	{
		fprintf(stderr, "Can not delete idb file.\n");
		return false;
	}

	if(err)
	{
		fprintf(stderr, "Can not delete idb file. err msg: %s\n", err.message().c_str());
		return false;
	}

	return true;
}

/**
 * @brief Function to write the offset of idb file into the iavf file.
 *
 * @param[in] iavf_file The iavf file.
 * @return true: Success, false: Failed.
 */
bool StreamingDownloader::write_idb_offset(boost::filesystem::fstream& iavf_file)
{
	// Open exception mode.
	iavf_file.exceptions(ifstream::failbit | ifstream::badbit);

	try
	{
		// Calculate the size of iavf file.
		iavf_file.seekp(0, std::ios_base::end);

		long size = iavf_file.tellp();
		if(size == -1)
		{
			fprintf(stderr, "[write_idb_offset]: Can not seek to get the file size.\n");
			return false;
		}

		// Seek to the offset 19 of iavf file then write the size to the iavf file.
		iavf_file.seekp(19, std::ios_base::beg);

		uint64_t size64 = size;
		iavf_file.write( (const char*)&size64, sizeof(uint64_t) );
	}
	catch(const std::ios_base::failure& err)
	{
		fprintf(stderr, "[write_idb_offset]: Catch exception: Error message: %s\n", err.what());
		// Close exception mode.
		iavf_file.exceptions(ifstream::goodbit);
		return false;
	}

	// Close exception mode.
	iavf_file.exceptions(ifstream::goodbit);
	return true;
}

/**
 * @brief Function to generate the MD5 checksum.
 *
 * @param[in] iavf_file The iavf file.
 * @param[inout] out_buffer The output result buffer.
 * @return true: Success, false: Failed.
 */
bool StreamingDownloader::generate_checksum(boost::filesystem::fstream& iavf_file, unsigned char* out_buffer)
{
	// Seek to the begin of file.
	iavf_file.seekg(0, std::ios_base::beg);
	if(iavf_file.fail() || iavf_file.bad())
	{
		fprintf(stderr, "[generate_checksum]: Can not seek to the begin of file.\n");
		return false;
	}

	// Initialize the context of MD5
	MD5_CTX md5_context;
	if(MD5_Init(&md5_context) != 1)
	{
		fprintf(stderr, "[generate_checksum]: Can not initialize the MD5 context!\n");
		return false;
	}

	unsigned char *in_buffer = new unsigned char[SIZE_OF_INPUT_BUFFER];

	// Collect the data for MD5
	while(!iavf_file.eof())
	{
		iavf_file.read((char*) in_buffer, SIZE_OF_INPUT_BUFFER);
		if(iavf_file.bad())
		{
			delete [] in_buffer;
			return false;
		}

		std::streamsize size = iavf_file.gcount();

		if(size > 0)
		{
			if(MD5_Update(&md5_context, in_buffer, size) != 1)
			{
				fprintf(stderr, "Can not update the MD5 context with chunks of the message!\n");
				MD5_Final(out_buffer, &md5_context); // Erase the MD5_CTX ??
				delete [] in_buffer;
				return false;
			}
		}
	}

	if(iavf_file.eof())
	{
		// Reset the error control state because the failed bit will be set when reach the end of file.
		iavf_file.clear();
	}

	delete [] in_buffer;

	// Generate the MD5 checksum of file.
	if(MD5_Final(out_buffer, &md5_context) != 1)
	{
		fprintf(stderr, "Can not get the final md5 checksum!\n");
		return false;
	}

	return true;
}

/**
 * @brief Function to append the idb file to the iavf file.
 *
 * @param[in] iavf_file The iavf file.
 * @param[in] idb_file The idb file.
 * @return true: Success, false: Failed.
 */
bool StreamingDownloader::append_idb_to_iavf(boost::filesystem::fstream& iavf_file, boost::filesystem::fstream& idb_file)
{
	// Seek to the end of iavf file.
	iavf_file.seekp(0, std::ios_base::end);
	if(iavf_file.fail() || iavf_file.bad())
	{
		fprintf(stderr, "[append_idb_to_iavf]: Can not seek to the end of file.\n");
		return false;
	}

	// Seek to the begin of idb file.
	idb_file.seekg(0, std::ios_base::beg);
	if(idb_file.fail() || idb_file.bad())
	{
		fprintf(stderr, "[append_idb_to_iavf]: Can not seek to the begin of file.\n");
		return false;
	}

	// Append the idb file to iavf file.
	unsigned char *in_buffer = new unsigned char[SIZE_OF_INPUT_BUFFER];

	while(!idb_file.eof())
	{
		idb_file.read((char*) in_buffer, SIZE_OF_INPUT_BUFFER);
		if(idb_file.bad())
		{
			delete [] in_buffer;
			return false;
		}

		std::streamsize size = idb_file.gcount();

		if(size > 0)
		{
			iavf_file.write((const char*) in_buffer, size);
			if(iavf_file.bad())
			{
				fprintf(stderr, "[append_idb_to_iavf]: Can not write the idb file to iavf file.\n");
				delete [] in_buffer;
				return false;
			}
		}
	}

	if(idb_file.eof())
	{
		// Reset the error control state because the failed bit will be set when reach the end of file.
		idb_file.clear();
	}

	delete [] in_buffer;
	return true;
}

/**
 * @brief Function to handle the packet from streaming client (The start frame will be I frame of each split file and each file has CSH and OSD packets in the start).
 *
 * @param[in] pkt_type The CPacket type.
 * @param[in] data The video and audio data.
 * @param[in] size The size of data.
 * @return
 * 1: continue
 * 0: success or complete(it may be success or failed when complete)
 * -1: error
 */
int StreamingDownloader::handle_packet(int pkt_type, unsigned char* data, size_t size)
{
#ifdef ONLY_DOWNLOAD
	if (pkt_type == 0) // instek::PACKET_TYPE_NONE
	{
		printf("get non packet.............................\n");
		if(callback_)
			callback_(0);
		return 0;
	}

	if (pkt_type == 7) // instek::PACKET_TYPE_CSSH
	{
//		printf("get csh packet-------------\n");
		return 1;
	}

	//packet timestamp
	uint32_t sec = *((uint32_t*)(data + size));
	uint32_t usec = (*((uint32_t*)(data + size + 4))) * 1000;
	instek::CTimeValue time_value(sec, usec);

	// If it completes the downloading jobs then do post-process and exit.
	if(sec > end_time_)
	{
		if(callback_)
			callback_(0);
		return 0;
	}

	// It is OSD.
	if (pkt_type == 8)
	{
//		printf("get OSD packet\n");
		return 1;
	}

	// It is I frame
	if (pkt_type == 2) // instek::PACKET_TYPE_I
	{
		// Call the progress callback.
		if(progress_callback_)
			progress_callback_(sec);

		return 1;
	}

//	printf("get packet type %d\n", pkt_type);
	return 1;
#else
	if (pkt_type == 0) // instek::PACKET_TYPE_NONE
	{
		printf("get non packet.............................\n");
		if(!this->post_process())
		{
			fprintf(stderr, "Can not do post-process for files.\n");
		}

		if(is_success_in_post_process())
		{
			if(callback_)
				callback_(0);
			return 0;
		}

		if(callback_)
			callback_(-1);
		return 0;
	}

	instek::CPacketType cpacket_type = (instek::CPacketType) pkt_type;

	if (pkt_type == 7) // instek::PACKET_TYPE_CSSH
	{
		//packet time stamp;
		uint32_t sec = *((uint32_t*)(data - 16));
		uint32_t usec = (*((uint32_t*)(data - 12))) * 1000;
		instek::CTimeValue time_value(sec, usec);
		// Update the information about the CSH packet then push it into list.
		csh_packet_.reset(new instek::CPacketBuf(cpacket_type, size, data, time_value));
		if(!csh_packet_)
		{
			fprintf(stderr, "Can not create packet for CSH.\n");
			if(callback_)
				callback_(-1);
			return -1;
		}
		packet_list_.push_back(csh_packet_);
		total_packet_size_in_list_ += csh_packet_->GetPacketSize();
	}
	else
	{
		//packet timestamp
		uint32_t sec = *((uint32_t*)(data + size));
		uint32_t usec = (*((uint32_t*)(data + size + 4))) * 1000;
		instek::CTimeValue time_value(sec, usec);

		// If it completes the downloading jobs then do post-process and exit.
		if(sec > end_time_)
		{
			if(packet_list_.size() > 0)
			{
				iavf_writer_->AppendDiscontinuousData(packet_list_);
				packet_list_.clear();
				total_packet_size_in_list_ = 0;
			}

			if(!this->post_process())
			{
				fprintf(stderr, "Can not do post-process for files.\n");
			}

			if(is_success_in_post_process())
			{
				if(callback_)
					callback_(0);
				return 0;
			}

			if(callback_)
				callback_(-1);
			return 0;
		}

		// Create new packet.
		instek::CPacketBuf_Ptr temp_packet(new instek::CPacketBuf(cpacket_type, size, data, time_value));
		if(!temp_packet)
		{
			fprintf(stderr, "Can not create packet.\n");
			if(callback_)
				callback_(-1);
			return -1;
		}

		// It is OSD.
		if (pkt_type == 8)
		{
			osd_packet_ = temp_packet;
		}

		// It is I frame
		if (pkt_type == 2) // instek::PACKET_TYPE_I
		{
			// Call the progress callback.
			if(progress_callback_)
				progress_callback_(sec);

			if(!iavf_writer_)
			{
				fprintf(stderr, "iavf writer is NULL.\n");
				if(callback_)
					callback_(-1);
				return -1;
			}

			// If the size meets the restriction then we create new file and flush old file.
			if( (iavf_writer_->GetUsedSize() + total_packet_size_in_list_) > split_file_size_)
			{
				if(!this->post_process())
				{
					fprintf(stderr, "Can not do post-process for files.\n");
				}
				if(!this->create_new_files())
				{
					fprintf(stderr, "Can not create new iavf and database file.\n");
					if(callback_)
						callback_(-1);
					return -1;
				}

				// Update the time stamp of CSH and OSD packet.
				if(!packet_list_.empty())
				{
					std::list< boost::shared_ptr<instek::CPacketBuf> >::iterator iter = packet_list_.begin();
					csh_packet_->SetTimestamp((*iter)->GetTimestamp());
					osd_packet_->SetTimestamp((*iter)->GetTimestamp());
				}
				// Push the CSH and OSD in the front of the list which does not be written into files.
				packet_list_.push_front(osd_packet_);
				packet_list_.push_front(csh_packet_);
				total_packet_size_in_list_ += (csh_packet_->GetPacketSize() + osd_packet_->GetPacketSize());
			}

			// Append the packets in the list into iavf writer then clear the list.
			if(packet_list_.size() > 0)
			{
				iavf_writer_->AppendDiscontinuousData(packet_list_);
				packet_list_.clear();
				total_packet_size_in_list_ = 0;
			}
		}

		// Push the packet into list.
		packet_list_.push_back(temp_packet);
		total_packet_size_in_list_ += temp_packet->GetPacketSize();
	}

	return 1;
#endif
}

/**
 * @brief Function for blocking call to download the files.
 *
 * @return true: Success, false: Failed.
 */
bool StreamingDownloader::Download()
{
	int pkt_type;
	unsigned char* data;
	size_t size;
	int status;
	int handle_packet_status;
	while(1)
	{
		status = streaming_client_->GetPacket(pkt_type, data, size);
		if(status <= 0)
		{
			if(status == -1)
				return false;
			::sleep(1);
			continue;
		}

		handle_packet_status = handle_packet(pkt_type, data, size);
		if(handle_packet_status == 1)
			continue;
		if(handle_packet_status == -1)
			return false;
		if(is_success_in_post_process())
			return true;

		return false;
	}
}

