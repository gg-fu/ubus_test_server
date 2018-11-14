cmake_minimum_required(VERSION 2.8)

aux_source_directory(./src/ DIRSRCS)
ADD_EXECUTABLE(server ${DIRSRCS})

target_include_directories(server PUBLIC
    "include" "/usr/local/libevent/include/" 
    "/usr/bin/libubox/include/libubox/" "/usr/local/ubus/incldue/")
#target_include_directories(GWProtocbuf PUBLIC
#    "/usr/local/protobuf/include")

target_link_libraries(server
      	-L"/usr/local/libevent/lib" event
	-L"/usr/bin/libubox/lib" blobmsg_json
	-L"/usr/bin/libubox/lib" ubox
	-L"/usr/local/ubus/lib"  ubus
	pthread
	)

install(TARGETS server RUNTIME DESTINATION bin)
