#include "CSem.h"
#include <chrono>
#include <mutex>
#include <condition_variable>


CSem::CSem(){
	m_count = 0;
}
void CSem::Signal(){
	std::lock_guard<decltype(m_mtx)> lock(m_mtx);
	++m_count;
	m_cv.notify_one();
}
void CSem::Wait(){
	std::unique_lock<decltype(m_mtx)> lock(m_mtx);
	while(!m_count)
		m_cv.wait(lock);
	--m_count;
}
