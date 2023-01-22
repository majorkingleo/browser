#ifndef THREAD_H
#define THREAD_H

#ifdef WIN32
#  include <process.h>
#  include <windows.h>
#else
# include <pthread.h>
#endif

#include <string>

#ifdef WIN32
inline void do_sleep( unsigned s ) { Sleep( s / 1000 ); }
#else
#include <unistd.h>
inline void do_sleep( unsigned s ) { usleep( s ); }
#endif


#ifdef WIN32
DWORD __stdcall run_thread( PVOID ptr );
#else
extern "C" void* run_thread( void* ptr );
#endif


#define LOCK_RETURN( type, value, mutex ) \
  mutex.lock(); \
  type  ret = value; \
  mutex.unlock(); \
  return ret

class Thread
{
public:
    
#ifdef WIN32
    typedef int             ThreadStruct;
#else
    typedef pthread_t       ThreadStruct;    
#endif

    struct Mutex
    {
#ifdef WIN32           
       struct MutexStruct
       {        
            static unsigned id;
            HANDLE       handle;
            std::string    name;
       };
#else
       typedef pthread_mutex_t MutexStruct;
#endif

	MutexStruct mutex;

	Mutex();
	~Mutex();

	void lock();
	void unlock();

	bool locked();
    };	

protected:

    ThreadStruct thread;

    Mutex        m_running;
    Mutex        m_start_count;
    int          start_count;

public:
    Thread() { start_count = 0; }
    virtual ~Thread();

    virtual void run() = 0;

    ThreadStruct& getThread() { return thread; }

    void start();
    bool running() { return m_running.locked(); }
    void wait() { m_running.lock(); m_running.unlock(); }
    int  getStartCount();

#ifdef WIN32
    friend DWORD __stdcall run_thread( PVOID ptr );
#else
    friend void* run_thread( void* ptr );
#endif

protected:
    
    void exit();

private:

    void runThread();
};



#endif
