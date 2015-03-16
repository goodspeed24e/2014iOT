#!/bin/sh

echo '=============== Start Test ======================'

echo '===============Test: Http Message Request======================='
./test-http-parser http_msg_files/request_1.txt 1

echo '===============Test: Http Message Response======================'
./test-http-parser http_msg_files/response_1.txt 2

echo '=============== End Test ======================'

