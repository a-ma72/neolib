// thread_pool.hpp
/*
 *  Copyright (c) 2017 Leigh Johnston.
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
#include <memory>
#include <vector>
#include <deque>
#include <future>
#include <mutex>
#include "i_thread.hpp"
#include "i_task.hpp"

namespace neolib
{
	class thread_pool
	{
		friend class thread_pool_thread;
	private:
		typedef std::vector<std::unique_ptr<i_thread>> thread_list;
		typedef std::pair<std::shared_ptr<i_task>, int32_t> task_queue_entry;
		typedef std::vector<task_queue_entry> task_queue;
	public:
		thread_pool();
	public:
		void reserve(std::size_t aMaxThreads);
		std::size_t active_threads() const;
		std::size_t available_threads() const;
		std::size_t total_threads() const;
		std::size_t max_threads() const;
		std::size_t waiting_tasks() const;
		std::size_t active_tasks() const;
	public:
		bool paused() const;
		void pause();
		void resume();
	public:
		void start(i_task& aTask, int32_t aPriority = 0);
		void start(std::shared_ptr<i_task> aTask, int32_t aPriority = 0);
		bool try_start(i_task& aTask);
		bool try_start(std::shared_ptr<i_task> aTask);
		std::future<void> run(std::function<void()> aFunction);
		template <typename T>
		std::future<T> run(std::function<T()> aFunction);
	public:
		static thread_pool& default_thread_pool();
	private:
		bool too_many_threads() const;
		task_queue::const_iterator free_slot(int32_t aPriority) const;
		void delete_task(i_task& aTask);
		void next_task();
	private:
		mutable std::recursive_mutex iMutex;
		std::size_t iMaxThreads;
		thread_list iThreads;
		task_queue iWaitingTasks;
		task_queue iActiveTasks;
		bool iPaused;
	};

	template <typename T>
	inline std::future<T> thread_pool::run(std::function<T()> aFunction)
	{
		auto newTask = std::make_shared<function_task<T>>(aFunction);
		start(newTask);
		return newTask->get_future();
	}
}