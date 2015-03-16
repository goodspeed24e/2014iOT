#include <iostream>
#include <libav/buf_mgr.hpp>
#include <cstdlib>
#include <deque>

using namespace std;

void* alloc_func(size_t size)
{
	cout << "Register allocate function.\n";
	return malloc(size);
}

void free_func(void* ptr)
{
	cout << "Register free function.\n";
	free(ptr);
}

int main()
{
	instek::BufMgr buf_mgr(alloc_func, free_func);
	//instek::BufMgr buf_mgr(malloc, free);

	// Test allocate buffer
	cout << "Test allocate buffer. ======================" << endl;
	instek::BufMgr::DataBuffer buf_1;
	buf_mgr.GetBuffer(20, buf_1);
	cout << "Size of buffer 1 = "  << buf_1.GetBufferSize() << endl;
	uint8_t* temp_ptr = (uint8_t*) buf_1.GetBuffer();
	if(temp_ptr)
		cout << "The pointer 1 is not NULL\n";

	instek::BufMgr::DataBuffer buf_2;
	buf_mgr.GetBuffer(20, buf_2);
	cout << "Size of buffer 2 = "  << buf_2.GetBufferSize() << endl;
	temp_ptr = (uint8_t*) buf_2.GetBuffer();
	if(temp_ptr)
		cout << "The pointer 2 is not NULL\n";

	instek::BufMgr::DataBuffer buf_3;
	buf_mgr.GetBuffer(10, buf_3);
	cout << "Size of buffer 3 = "  << buf_3.GetBufferSize() << endl;
	temp_ptr = (uint8_t*) buf_3.GetBuffer();
	if(temp_ptr)
		cout << "The pointer 3 is not NULL\n";

	uint8_t* temp_buf4 = NULL;
	size_t temp_buf4_size = 23;
	buf_mgr.GetBuffer(temp_buf4_size, (void**)&temp_buf4);
	if(temp_buf4)
		cout << "The pointer 4 is not NULL\n";

	instek::BufMgr::BufferListContainer temp_list;	
	uint8_t* temp_buf5 = NULL;
	size_t temp_buf5_size = 24;
	buf_mgr.GetBuffer(temp_buf5_size, (void**)&temp_buf5);
	if(temp_buf5)
		cout << "The pointer 5 is not NULL\n";
	temp_list.push_back(make_pair(temp_buf5_size, temp_buf5));

	temp_buf5 = NULL;
	temp_buf5_size = 25;
	buf_mgr.GetBuffer(temp_buf5_size, (void**)&temp_buf5);
	if(temp_buf5)
		cout << "The pointer 5 is not NULL\n";
	temp_list.push_back(make_pair(temp_buf5_size, temp_buf5));


	// Test ReleaseBuffer
	cout << "Test release buffer. ======================" << endl;
	buf_mgr.ReleaseBuffer(buf_2);
	cout << "Size of buffer 2 = "  << buf_2.GetBufferSize() << " after release." << endl;
	temp_ptr = (uint8_t*) buf_2.GetBuffer();
	if(!temp_ptr)
		cout << "The pointer 2 is NULL after release\n";

	buf_mgr.ReleaseBuffer(temp_buf4_size, (void**)&temp_buf4);
	if(!temp_buf4)
		cout << "The pointer 4 is NULL after release\n";

	buf_mgr.ReleaseBuffer(temp_list);
	if(temp_list.empty())
		cout << "The buffer list is empty after release\n";


	// Test get the buffer in the queue
	cout << "Test get buffer in the queue. ======================" << endl;
	buf_mgr.GetBuffer(20, buf_2);
	cout << "Size of buffer 2 = "  << buf_2.GetBufferSize() << endl;
	temp_ptr = (uint8_t*) buf_2.GetBuffer();
	if(temp_ptr)
		cout << "The pointer 2 is not NULL\n";

	// Test free all buffer
	cout << "Test free all buffer. ======================" << endl;
	buf_mgr.ReleaseBuffer(buf_1);
	buf_mgr.ReleaseBuffer(buf_2);
	buf_mgr.ReleaseBuffer(buf_3);
	buf_mgr.FreeAllAvailableBuffer();

	// Prepare buffer for testing free function
	cout << "Prepare buffer for testing free function. ======================" << endl;
	std::deque<instek::BufMgr::DataBuffer> data_buf_vec;

	for(int i = 1; i < 8; ++i)
	{
		instek::BufMgr::DataBuffer temp_buf;

		buf_mgr.GetBuffer(i, temp_buf);
		data_buf_vec.push_back(temp_buf);

		buf_mgr.GetBuffer(i, temp_buf);
		data_buf_vec.push_back(temp_buf);
	}

	for(size_t i = 0; i < data_buf_vec.size(); ++i)
	{
		buf_mgr.ReleaseBuffer(data_buf_vec[i]);
	}
	data_buf_vec.clear();

	// Test free specific size buffer
	cout << "Test free specific size buffer. (Free 2, 4, 8) ======================" << endl;
	std::deque<size_t> size_vec;
	size_vec.push_back(2);
	size_vec.push_back(4);
	size_vec.push_back(8); // non-exist index
	buf_mgr.FreeSpecificSize(size_vec);

	// Test reserve specific size buffer and free others
	cout << "Test reserve specific size buffer and free others. (Reserver 3, 6, 9) ======================" << endl;
	size_vec.clear();
	size_vec.push_back(6);
	size_vec.push_back(3);
	size_vec.push_back(9); // non-exist index
	buf_mgr.ReserveSpecificSizeAndFreeOthers(size_vec);

	// At last, we free all
	cout << "At last, free all buffer in the destructor of BufMgr. ======================" << endl;
	return 0;
}
