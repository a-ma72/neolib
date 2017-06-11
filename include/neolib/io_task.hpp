// io_task.hpp v1.0
/*
 *  Copyright (c) 2012-2017 Leigh Johnston.
 *
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 *     * Neither the name of Leigh Johnston nor the names of any
 *       other contributors to this software may be used to endorse or
 *       promote products derived from this software without specific prior
 *       written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 *  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "neolib.hpp"
#include <boost/asio.hpp>
#include "i_thread.hpp"
#include "i_task.hpp"
#include "message_queue.hpp"

namespace neolib
{
	class io_task;

	class io_service
	{
		// types
	public:
		typedef boost::asio::io_service native_io_service_type;
	public:
		io_service(io_task& aTask) : iTask(aTask) {}
		// operations
	public:
		bool do_io(bool aProcessEvents = true);
		native_io_service_type& native_object() { return iNativeIoService; }
		// attributes
	private:
		io_task& iTask;
		native_io_service_type iNativeIoService;
	};

	enum class yield_type
	{
		NoYield,
		Yield,
		Sleep
	};

	class io_task : public i_task
	{
		// types
	private:
		typedef std::unique_ptr<neolib::message_queue> message_queue_pointer;
		// exceptions
	public:
		struct no_message_queue : std::logic_error { no_message_queue() : std::logic_error("neolib::io_task::no_message_queue") {} };
		// construction
	public:
		io_task(i_thread& aThread, const std::string& aName = "");
		// operations
	public:
		const std::string& name() const override;
		i_thread& thread() const;
		bool do_io(yield_type aYieldIfNoWork = yield_type::NoYield);
		io_service& timer_io_service() { return iTimerIoService; }
		io_service& networking_io_service() { return iNetworkingIoService; }
		bool have_message_queue() const;
		bool have_messages() const;
		void create_message_queue(std::function<bool()> aIdleFunction = std::function<bool()>());
		const neolib::message_queue& message_queue() const;
		neolib::message_queue& message_queue();
		bool pump_messages();
		bool halted() const;
		void halt();
		// implementation
	public:
		void run() override;
		// attributes
	private:
		i_thread& iThread;
		std::string iName;
		io_service iTimerIoService;
		io_service iNetworkingIoService;
		message_queue_pointer iMessageQueue;
		bool iHalted;
	};
}
