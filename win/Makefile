

CC = g++

CFLAGS = -Wall -O -g -DCANDY_HAVE_WINDOWS

INCLUDE_PATH = -I../include/

BUILD_PATH = ./build/

OBJ_PATH = $(BUILD_PATH)obj/

SRC_PATH = ../src/

TEST_PATH = ../examples/

SHARED = -fPIC -shared

LIB_PATH = -L./build/
#-fPIC 作用于编译阶段，告诉编译器产生与位置无关代码(Position-Independent Code)，
 # 则产生的代码中，没有绝对地址，全部使用相对地址，故而代码可以被加载器加载到内存的任意
#  位置，都可以正确的执行。这正是共享库所要求的，共享库被加载时，在内存的位置不是固定的。

all:$(BUILD_PATH)libcandy.dll $(BUILD_PATH)echo_server.exe $(BUILD_PATH)echo_client.exe
OBJS= 	$(OBJ_PATH)candy_context.o\
		$(OBJ_PATH)candy_array.o\
		$(OBJ_PATH)candy_glock.o\
		$(OBJ_PATH)candy_log.o\
		$(OBJ_PATH)candy_queue.o\
		$(OBJ_PATH)candy_thread.o\
		$(OBJ_PATH)candy_timerset.o\
		$(OBJ_PATH)candy_aio.o\
		$(OBJ_PATH)candy_poller.o\
		$(OBJ_PATH)candy_socket.o\
		$(OBJ_PATH)candy_worker.o\
		$(OBJ_PATH)candy_worker_pool.o\
		$(OBJ_PATH)candy_sleep.o\
		$(OBJ_PATH)candy.o\
		$(OBJ_PATH)candy_aio_pool.o\
		$(OBJ_PATH)candy_cycle_buffer.o\
		$(OBJ_PATH)candy_thread_pool.o\
		$(OBJ_PATH)candy_buffer.o\
		$(OBJ_PATH)candy_net_int.o\
		$(OBJ_PATH)candy_timer_pool.o
	
$(OBJ_PATH)candy_net_int.o : $(SRC_PATH)base/candy_net_int.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^	

$(OBJ_PATH)candy.o : $(SRC_PATH)aio/candy.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^
	
$(OBJ_PATH)candy_context.o : $(SRC_PATH)aio/candy_context.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^

$(OBJ_PATH)candy_array.o : $(SRC_PATH)base/candy_array.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^

$(OBJ_PATH)candy_cycle_buffer.o : $(SRC_PATH)base/candy_cycle_buffer.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^

$(OBJ_PATH)candy_buffer.o : $(SRC_PATH)base/candy_buffer.c
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

$(OBJ_PATH)candy_thread_pool.o : $(SRC_PATH)base/candy_thread_pool.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^

$(OBJ_PATH)candy_timerset.o : $(SRC_PATH)aio/candy_timerset.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^

$(OBJ_PATH)candy_aio.o : $(SRC_PATH)aio/candy_aio.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^

$(OBJ_PATH)candy_aio_pool.o : $(SRC_PATH)aio/candy_aio_pool.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^

$(OBJ_PATH)candy_timer_pool.o : $(SRC_PATH)aio/candy_timer_pool.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^

$(OBJ_PATH)candy_poller.o : $(SRC_PATH)aio/candy_poller.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^

$(OBJ_PATH)candy_socket.o : $(SRC_PATH)aio/candy_socket.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^

$(OBJ_PATH)candy_worker.o : $(SRC_PATH)aio/candy_worker.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^

$(OBJ_PATH)candy_worker_pool.o : $(SRC_PATH)aio/candy_worker_pool.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^

$(OBJ_PATH)echo_server.o : $(TEST_PATH)/echo_server.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^

$(OBJ_PATH)echo_client.o : $(TEST_PATH)/echo_client.c
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c -o $@  $^

$(BUILD_PATH)echo_server.exe: $(OBJ_PATH)echo_server.o
	$(CC) $(INCLUDE_PATH) $(CFLAGS)  -o $@  $^ -lwsock32 -lcandy $(LIB_PATH)
$(BUILD_PATH)echo_client.exe: $(OBJ_PATH)echo_client.o
	$(CC) $(INCLUDE_PATH) $(CFLAGS)  -o $@  $^ -lwsock32 -lcandy $(LIB_PATH)
$(BUILD_PATH)libcandy.dll:$(OBJS)
	$(CC) $(CFLAGS) $(INC_DIR) $(SHARED) -o $@ $^   -lwsock32

