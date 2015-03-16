#ifndef STREAMING_DOWNLOADER_HPP
#define STREAMING_DOWNLOADER_HPP

#include <libstreaming/streaming_client.hpp>
#include <util/packet_buf.hpp>
#include <iavf_ng/avfile_writer.hpp>
#include <list>
#include <boost/function.hpp>
#include <boost/filesystem/fstream.hpp>

/**
 * Callback function for downloading status.
 * param:
 * 0: Success (completed).
 * -1: Failed.
 * -2: Network error.
 */
typedef boost::function< void(int) > DownloadStatusCallback;

/**
 * Callback function for downloading progress.
 *
 * param: The second (sec) of I frame.
 */
typedef boost::function< void(unsigned int) > DownloadProgressCallback;

/// Class for downloading the iavf file.
class StreamingDownloader
{
public:
	StreamingDownloader(const instek::StreamingClient::StreamingInfo& s_info,
			unsigned int end_time,
			const std::string& iavf_output_path,
			const std::string& idb_output_path,
			const std::string& output_filename,
			size_t split_file_size,
			int time_zone,
			const DownloadStatusCallback& callback,
			const DownloadProgressCallback& progress_callback);
	~StreamingDownloader();

	bool Download();

private:
	bool create_new_files();
	bool is_success_in_post_process();
	bool post_process();
	bool complete_iavf(const std::string& iavf_path, const std::string& idb_path, iavfng::CAVFileWriter*);
	bool write_idb_offset(boost::filesystem::fstream& iavf_file);
	bool generate_checksum(boost::filesystem::fstream& iavf_file, unsigned char* out_buffer);
	bool append_idb_to_iavf(boost::filesystem::fstream& iavf_file, boost::filesystem::fstream& idb_file);
	int handle_packet(int pkt_type, unsigned char* data, size_t size);
	void streaming_err_callback_func(int);

private:
	/// Streaming client.
	boost::shared_ptr<instek::StreamingClient> streaming_client_;

	/// The list to store the CPacket from stremaing client.
	std::list< boost::shared_ptr<instek::CPacketBuf> > packet_list_;

	/// iavf writer.
	iavfng::CAVFileWriter* iavf_writer_;

	/// CSH packet.
	instek::CPacketBuf_Ptr csh_packet_;

	/// OSD packet.
	instek::CPacketBuf_Ptr osd_packet_;

	/// Streaming information for streaming client.
	instek::StreamingClient::StreamingInfo streaming_info_;

	/// End time of iavf file for downloading.
	unsigned int end_time_;

	/// The IAVF output filename and path (excluding the file extension).
	std::string iavf_output_filepath_;

	/// The IDB output filename and path (excluding the file extension).
	std::string idb_output_filepath_;

	/// The path and filename of current iavf file (including the index and the file extension).
	std::string current_iavf_filepath_;

	/// The path and filename of current idb file (including the index and the file extension).
	std::string current_idb_filepath_;

	/// The max size of each split file.
	size_t split_file_size_;

	/// The time zone.
	int time_zone_;

	/// The file index of split file.
	unsigned int file_index_;

	/// The callback function for downloading status.
	DownloadStatusCallback callback_;

	/// The callback function for downloading progress.
	DownloadProgressCallback progress_callback_;

	/// The size of total packet in the list which have not appended into the writer.
	size_t total_packet_size_in_list_;
};

#endif
