set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set (CANDY_SOURCES
    candy.h
    aio/candy_aio.h
    aio/candy_aio.c
    aio/candy_aio_pool.h
    aio/candy_aio_pool.c
    aio/candy_poller.h
    aio/candy_poller.c
    aio/candy_poller_epoll.h
    aio/candy_poller_epoll.inc
    aio/candy_poller_select.h
    aio/candy_poller_select.inc
    aio/candy_socket.h
    aio/candy_socket.c
    aio/candy_worker.h
    aio/candy_worker.c
    aio/candy_worker_pool.h
    aio/candy_worker_pool.c
    base/candy_array.h
    base/candy_array.c
    base/candy_cycle_buffer.c
    base/candy_cycle_buffer.h
    base/candy_error.h
    base/candy_export.h
    base/candy_glock.c
    base/candy_glock.h
    base/candy_log.h
    base/candy_log.c
    base/candy_queue.h
    base/candy_queue.c
    base/candy_sleep.h
    base/candy_sleep.c
    base/candy_thread.h
    base/candy_thread.c
    base/candy_thread_posix.h
    base/candy_thread_posix.inc
    base/candy_thread_win.h
    base/candy_thread_win.inc
    base/candy_window.h
    base/candy_timer.h
    base/candy_timer.c

    core/candy.c
    core/candy_context.c
    core/candy_context.h
)

add_library (candy SHARED ${CANDY_SOURCES})

add_definitions (-DCANDY_EXPORTS)

target_link_libraries (candy ws2_32)
target_link_libraries (candy Mswsock.lib)


install(TARGETS candy
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)