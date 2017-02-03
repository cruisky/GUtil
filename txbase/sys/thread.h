#pragma once

#include "txbase/fwddecl.h"

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <atomic>

namespace TX{
	/// <summary>
	/// TaskScheduler that assigns tasks to worker threads.
	/// </summary>
	class TaskScheduler {
	public:
		/// <summary>
		/// Worker thread that gets tasks from the scheduler.
		/// </summary>
		class Worker : public std::thread {
		public:
			Worker(TaskScheduler *scheduler, int id) :
				std::thread(&Worker::WorkLoop, this),
				scheduler_(scheduler),
				id_(id)
				{}
		private:
			void WorkLoop();
		private:
			TaskScheduler *scheduler_;
			int id_;
		};

		/// <summary>
		/// Callable function.
		/// </summary>
		class Task {
		public:
			typedef void(*Func)(void *args, int id);

			Task() : func(nullptr), args(nullptr){}
			Task(Func func, void *args) : func(func), args(args){}
			Task(const Task& other) : func(other.func), args(other.args){}
			inline Task& operator = (const Task& other) { this->func = other.func; this->args = other.args; }

			inline void Run(int id){ func(args, id); }
		private:
			Func func;
			void *args;
		};


	private:
		TaskScheduler() : running_(false) { task_count_ = 0; }
	public:
		static TaskScheduler *Instance();
		static void DeleteInstance();
	public:
		inline bool Running(){ return running_; }
		inline int ThreadCount(){ return threads.size(); }
		void StartAll();
		void StopAll();
		void AddTask(Task& newTask);
		void JoinAll();

	public:
		std::deque<Task> tasks;
		std::vector<Worker> threads;

	private:
		static TaskScheduler *instance;
		std::atomic_bool running_;
		std::atomic_int task_count_;

		std::mutex tasks_mutex_;
		std::condition_variable task_available_cv_;

		std::mutex finished_mutex_;
		std::condition_variable task_finished_cv_;	// the completion of one task
	};
}
