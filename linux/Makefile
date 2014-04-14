

CC = gcc

CFLAGS = -Wall -O -g -DCANDY_USE_EPOLL -fPIC
#-std=c++0x 使用c++ 0x 标准
INCLUDE_PATH = -I../src/

BUILD_PATH = ./build/

OBJ_PATH = $(BUILD_PATH)obj/

SRC_PATH = ../src/

SHARED = -fPIC -shared

LIB_PATH = -L./build/
#-fPIC 作用于编译阶段，告诉编译器产生与位置无关代码(Position-Independent Code)，
 # 则产生的代码中，没有绝对地址，全部使用相对地址，故而代码可以被加载器加载到内存的任意
#  位置，都可以正确的执行。这正是共享库所要求的，共享库被加载时，在内存的位置不是固定的。

all:$(BUILD_PATH)libcandy.so $(BUILD_PATH)test_server.exe $(BUILD_PATH)test_client.exe
install:
	cp $(BUILD_PATH)libcandy.so /usr/lib64/
	cp $(SRC_PATH)candy.h $(SRC_PATH)base/candy_array.h $(SRC_PATH)base/candy_queue.h $(SRC_PATH)base/candy_cycle_buffer.h  /include
clean:
	rm -rf $(OBJ_PATH)*
OBJS= 	$(OBJ_PATH)candy_context.o\
		$(OBJ_PATH)candy_array.o\
		$(OBJ_PATH)candy_glock.o\
		$(OBJ_PATH)candy_log.o\
		$(OBJ_PATH)candy_queue.o\
		$(OBJ_PATH)candy_thread.o\
		$(OBJ_PATH)candy_timer.o\
		$(OBJ_PATH)candy_aio.o\
		$(OBJ_PATH)candy_poller.o\
		$(OBJ_PATH)candy_socket.o\
		$(OBJ_PATH)candy_worker.o\
		$(OBJ_PATH)candy_worker_pool.o\
		$(OBJ_PATH)candy_sleep.o\
		$(OBJ_PATH)candy.o\
		$(OBJ_PATH)candy_aio_pool.o\
		$(OBJ_PATH)candy_cycle_buffer.o
		
$(OBJ_PATH)candy_cycle_buffer.o : $(SRC_PATH)base/candy_cycle_buffer.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^

$(OBJ_PATH)candy_worker_pool.o : $(SRC_PATH)aio/candy_worker_pool.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^

$(OBJ_PATH)candy_context.o : $(SRC_PATH)core/candy_context.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^

$(OBJ_PATH)candy.o : $(SRC_PATH)core/candy.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^

$(OBJ_PATH)candy_array.o : $(SRC_PATH)base/candy_array.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^

$(OBJ_PATH)candy_glock.o : $(SRC_PATH)base/candy_glock.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^

$(OBJ_PATH)candy_sleep.o : $(SRC_PATH)base/candy_sleep.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^

$(OBJ_PATH)candy_log.o : $(SRC_PATH)base/candy_log.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^

$(OBJ_PATH)candy_queue.o : $(SRC_PATH)base/candy_queue.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^

$(OBJ_PATH)candy_thread.o : $(SRC_PATH)base/candy_thread.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^

$(OBJ_PATH)candy_timer.o : $(SRC_PATH)base/candy_timer.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^

$(OBJ_PATH)candy_aio.o : $(SRC_PATH)aio/candy_aio.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^
$(OBJ_PATH)candy_aio_pool.o : $(SRC_PATH)aio/candy_aio_pool.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^
$(OBJ_PATH)candy_poller.o : $(SRC_PATH)aio/candy_poller.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^

$(OBJ_PATH)candy_socket.o : $(SRC_PATH)base/candy_socket.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^

$(OBJ_PATH)candy_worker.o : $(SRC_PATH)aio/candy_worker.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^

$(OBJ_PATH)server.o : $(SRC_PATH)test/server.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^

$(OBJ_PATH)client.o : $(SRC_PATH)test/client.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^

$(BUILD_PATH)test_server.exe: $(OBJ_PATH)server.o
	$(CC) $(INCLUDE_PATH) $(CFLAGS)  -o $@  $^ -lpthread -lcandy $(LIB_PATH) -Wl,-rpath,$(BUILD_PATH)
$(BUILD_PATH)test_client.exe: $(OBJ_PATH)client.o
	$(CC) $(INCLUDE_PATH) $(CFLAGS)  -o $@  $^ -lpthread -lcandy $(LIB_PATH) -Wl,-rpath,$(BUILD_PATH)
$(BUILD_PATH)libcandy.so:$(OBJS)
	$(CC) $(CFLAGS) $(INC_DIR) $(SHARED) -o $@ $^   -lpthread

