#ifndef __rpcdemo_service_h__
#define __rpcdemo_service_h__

#include <zce/zce_service.h>
#include <zce/zce_thread.h>
#include <zce/zce_types.h>
#include <map>

class zce_reactor;
class zce_timer;
class zvm;

class rpcdemo_service : public zce_service
{
    zce_smartptr<zvm> vmptr_;
	zce_smartptr<zce_timer> timer_;
    zce_smartptr<zce_reactor> reactor_ptr_;
	unsigned timer_count_;
public:
    
    rpcdemo_service();

    zce_int64 service_seq() const {
        return 1;
    }

	void handle_timeout(const zce_any& arg);

    void test_rpcdemo(const zce_smartptr<zce_object>& obj);
private:

    bool on_start(int argc, char* argv[]);

    bool on_stop();
};


extern rpcdemo_service* g_service;

#endif // __rpcdemo_service_h__