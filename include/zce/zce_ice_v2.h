/* ***************************************************************
//  rv_ice   version:  1.0  date: 18/12/2009 
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  Copyright (C) 2009 - All Rights Reserved
// ***************************************************************
// 
// **************************************************************/
#ifndef __rv_ice_h__
#define __rv_ice_h__

//#include <zce/zce_methodreq.h>
#include <Ice/Handle.h>
#include <IceUtil/Shared.h>
#include <Ice/Exception.h>
#include <Ice/LocalException.h>
#include <Ice/Current.h>

#include <zce/zce_object.h>
#include <zce/zce_task.h>
#include <zce/zce_task_queue.h>
#include <set>

class zce_task_queue;

namespace zce
{
    template <typename CALLBACK_CTX, typename ARG0_TYPE, typename ARG1_TYPE>
    class rvice_task_impl : public zce_task
    {
        typedef rvice_task_impl<CALLBACK_CTX, ARG0_TYPE, ARG1_TYPE> impl_class;
        CALLBACK_CTX cbctx_;
        int result_;
        ARG0_TYPE arg0_;
        ARG1_TYPE arg1_;
        const char* desc_;

        int enqueue(const zce_smartptr<zce_task_queue>& tskdeque_ctx, const char* name = "rvice_task_base") {
			zce_smartptr<zce_task> task_ptr(this);
			if (tskdeque_ctx != 0)
				return tskdeque_ctx->enqueue(task_ptr);
			else
				return zce_schedule_sigt::instance()->perform(task_ptr);
        }

		rvice_task_impl(const CALLBACK_CTX& ctx, const ARG1_TYPE& arg1, const char* desc)
			:zce_task("rvice_task_impl"), cbctx_(ctx), arg1_(arg1), desc_(desc), result_(0)
		{
			ZCE_ASSERT(ctx != 0);
		}

    public:
        static zce_smartptr<impl_class> create(
            const CALLBACK_CTX& ctx, const ARG1_TYPE& arg1, const char* desc) {
            return zce_smartptr<impl_class>(new impl_class(ctx, arg1, desc));
        }

        ~rvice_task_impl()
        {
        }

        virtual void ice_response()
        {
            result_ = 0;
            this->enqueue(cbctx_->get_tskdeque(), desc_);
        }

        virtual void ice_response(::Ice::Int result)
        {
            result_ = result;
            this->enqueue(cbctx_->get_tskdeque(), desc_);
        }

        //virtual void ice_response(::Ice::Int result, ARG0_TYPE arg0)
        //{
        //    result_ = result;
        //    arg0_ = arg0;
        //    this->enqueue(cbctx_->get_tskdeque(), desc_);
        //}

		virtual void ice_response(::Ice::Int result, const ARG0_TYPE& arg0)
		{
			result_ = result;
			arg0_ = arg0;
			this->enqueue(cbctx_->get_tskdeque(), desc_);
		}

        virtual void ice_exception(const std::exception_ptr& ex)
        {
            //ZCE_ERROR((ZLOG_ERROR, "rvice_task_impl(%s) exception %s\n", desc_, ex.what()));
            result_ = 0x80010004; /*ERV_ERRO_CMN_UNAVAILSERVICE*/
            this->enqueue(cbctx_->get_tskdeque(), desc_);
        }

        virtual void call(void) {
			cbctx_->rvice_execute(result_, arg0_, arg1_);
        }
    };

    template <typename CALLBACK_CTX, typename ARG0_TYPE, typename ARG1_TYPE, typename ARG_TYPE>
    class rvice_task_impl_2 :public zce_task
    {
        typedef rvice_task_impl_2<CALLBACK_CTX, ARG0_TYPE, ARG1_TYPE, ARG_TYPE> impl_class;
        CALLBACK_CTX cbctx_;
        int result_;
        ARG0_TYPE arg0_;
        ARG1_TYPE arg1_;
        ARG_TYPE arg_;
        const char* desc_;

		int enqueue(const zce_smartptr<zce_task_queue>& tskdeque_ctx, const char* name = "rvice_task_base") {
			zce_smartptr<zce_task> task_ptr(this);
			if (tskdeque_ctx != 0)
				return tskdeque_ctx->enqueue(task_ptr);
			else
				return zce_schedule_sigt::instance()->perform(task_ptr);
		}

        rvice_task_impl_2(const CALLBACK_CTX& ctx, const ARG_TYPE& arg, const char* desc = 0)
			:zce_task("rvice_task_impl_2"), cbctx_(ctx), arg_(arg), desc_(desc), result_(0)
		{
			ZCE_ASSERT(ctx != 0);
		}

    public:

		static zce_smartptr<impl_class> create(
			const CALLBACK_CTX& ctx, const ARG_TYPE& arg, const char* desc = 0) {
			return zce_smartptr<impl_class>(new impl_class(ctx, arg, desc));
		}

        ~rvice_task_impl_2()
        {
        }

        virtual void ice_response()
        {
            result_ = 0;
            this->enqueue(cbctx_->get_tskdeque());
        }

        virtual void ice_response(::Ice::Int result)
        {
            result_ = result;
            this->enqueue(cbctx_->get_tskdeque());
        }

        //virtual void ice_response(::Ice::Int result, ARG0_TYPE arg0, ARG1_TYPE arg1)
        //{
        //    result_ = result;
        //    arg0_ = arg0;
        //    arg1_ = arg1;
        //    this->enqueue(cbctx_->get_tskdeque());
        //}

        //virtual void ice_response(::Ice::Int result, ARG0_TYPE arg0, const ARG1_TYPE& arg1)
        //{
        //    result_ = result;
        //    arg0_ = arg0;
        //    arg1_ = arg1;
        //    this->enqueue(cbctx_->get_tskdeque());
        //}

        //virtual void ice_response(::Ice::Int result, const ARG0_TYPE& arg0, ARG1_TYPE arg1)
        //{
        //    result_ = result;
        //    arg0_ = arg0;
        //    arg1_ = arg1;
        //    this->enqueue(cbctx_->get_tskdeque());
        //}

        virtual void ice_response(::Ice::Int result, const ARG0_TYPE& arg0, const ARG1_TYPE& arg1)
        {
            result_ = result;
            arg0_ = arg0;
            arg1_ = arg1;
            this->enqueue(cbctx_->get_tskdeque());
        }

        virtual void ice_exception(const ::std::exception_ptr& ex)
        {
            //ZCE_ERROR((ZLOG_ERROR, "rvice_task_impl(%s) exception %s\n", desc_, ex.what()));
            result_ = 0x80010004; /*ERV_ERRO_CMN_UNAVAILSERVICE*/
            this->enqueue(cbctx_->get_tskdeque());
        }

		virtual void call(void) {
			cbctx_->rvice_execute(result_, arg0_, arg1_, arg_);
		}
    };

    class rv_ipclass
    {
        struct route_entry {
            unsigned netaddr;
            unsigned netmask;
            unsigned short mask;
            unsigned short ipclass;
            bool operator<(const route_entry& rhs) const;
        };

        std::set<route_entry> rt_set_;
    public:
        int load_ipclass(const char* fname, int ipclass);
        unsigned get_ipclass(unsigned ip);
    };

    std::string get_ice_local_address(const Ice::Current& cur);
    std::string get_ice_local_ip(const Ice::Current& cur);
    unsigned short get_ice_local_port(const Ice::Current& cur);

    std::string get_ice_remote_address(const Ice::Current& cur);
    std::string get_ice_remote_ip(const Ice::Current& cur);
    unsigned short get_ice_remote_port(const Ice::Current& cur);
}

#endif // __rv_ice_h__
