//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.lslboost.org/LICENSE_1_0.txt)
//
// See http://www.lslboost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERPROCESS_CONDITION_HPP
#define BOOST_INTERPROCESS_CONDITION_HPP

#ifndef BOOST_CONFIG_HPP
#  include <lslboost/config.hpp>
#endif
#
#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#if !defined(BOOST_INTERPROCESS_DOXYGEN_INVOKED)

#include <lslboost/interprocess/detail/config_begin.hpp>
#include <lslboost/interprocess/detail/workaround.hpp>

#include <lslboost/interprocess/detail/posix_time_types_wrk.hpp>
#include <lslboost/interprocess/sync/interprocess_mutex.hpp>
#include <lslboost/interprocess/sync/detail/locks.hpp>
#include <lslboost/interprocess/exceptions.hpp>
#include <lslboost/limits.hpp>
#include <lslboost/assert.hpp>

#if !defined(BOOST_INTERPROCESS_FORCE_GENERIC_EMULATION) && defined(BOOST_INTERPROCESS_POSIX_PROCESS_SHARED)
   #include <lslboost/interprocess/sync/posix/condition.hpp>
   #define BOOST_INTERPROCESS_USE_POSIX
//Experimental...
#elif !defined(BOOST_INTERPROCESS_FORCE_GENERIC_EMULATION) && defined (BOOST_INTERPROCESS_WINDOWS)
   #include <lslboost/interprocess/sync/windows/condition.hpp>
   #define BOOST_INTERPROCESS_USE_WINDOWS
#elif !defined(BOOST_INTERPROCESS_DOXYGEN_INVOKED)
   #include <lslboost/interprocess/sync/spin/condition.hpp>
   #define BOOST_INTERPROCESS_USE_GENERIC_EMULATION
#endif

#endif   //#ifndef BOOST_INTERPROCESS_DOXYGEN_INVOKED

//!\file
//!Describes process-shared variables interprocess_condition class

namespace lslboost {

#if !defined(BOOST_INTERPROCESS_DOXYGEN_INVOKED)

namespace posix_time
{  class ptime;   }

#endif   //#if !defined(BOOST_INTERPROCESS_DOXYGEN_INVOKED)

namespace interprocess {

class named_condition;

//!This class is a condition variable that can be placed in shared memory or
//!memory mapped files.
//!Destroys the object of type std::condition_variable_any
//!
//!Unlike std::condition_variable in C++11, it is NOT safe to invoke the destructor if all
//!threads have been only notified. It is required that they have exited their respective wait
//!functions.
class interprocess_condition
{
   #if !defined(BOOST_INTERPROCESS_DOXYGEN_INVOKED)
   //Non-copyable
   interprocess_condition(const interprocess_condition &);
   interprocess_condition &operator=(const interprocess_condition &);
   friend class named_condition;
   #endif   //#ifndef BOOST_INTERPROCESS_DOXYGEN_INVOKED

   public:
   //!Constructs a interprocess_condition. On error throws interprocess_exception.
   interprocess_condition()
   {}

   //!Destroys *this
   //!liberating system resources.
   ~interprocess_condition()
   {}

   //!If there is a thread waiting on *this, change that
   //!thread's state to ready. Otherwise there is no effect.
   void notify_one()
   {  m_condition.notify_one();  }

   //!Change the state of all threads waiting on *this to ready.
   //!If there are no waiting threads, notify_all() has no effect.
   void notify_all()
   {  m_condition.notify_all();  }

   //!Releases the lock on the interprocess_mutex object associated with lock, blocks
   //!the current thread of execution until readied by a call to
   //!this->notify_one() or this->notify_all(), and then reacquires the lock.
   template <typename L>
   void wait(L& lock)
   {
      ipcdetail::internal_mutex_lock<L> internal_lock(lock);
      m_condition.wait(internal_lock);
   }

   //!The same as:
   //!while (!pred()) wait(lock)
   template <typename L, typename Pr>
   void wait(L& lock, Pr pred)
   {
      ipcdetail::internal_mutex_lock<L> internal_lock(lock);
      m_condition.wait(internal_lock, pred);
   }

   //!Releases the lock on the interprocess_mutex object associated with lock, blocks
   //!the current thread of execution until readied by a call to
   //!this->notify_one() or this->notify_all(), or until time abs_time is reached,
   //!and then reacquires the lock.
   //!Returns: false if time abs_time is reached, otherwise true.
   template <typename L>
   bool timed_wait(L& lock, const lslboost::posix_time::ptime &abs_time)
   {
      ipcdetail::internal_mutex_lock<L> internal_lock(lock);
      return m_condition.timed_wait(internal_lock, abs_time);
   }

   //!The same as:   while (!pred()) {
   //!                  if (!timed_wait(lock, abs_time)) return pred();
   //!               } return true;
   template <typename L, typename Pr>
   bool timed_wait(L& lock, const lslboost::posix_time::ptime &abs_time, Pr pred)
   {
      ipcdetail::internal_mutex_lock<L> internal_lock(lock);
      return m_condition.timed_wait(internal_lock, abs_time, pred);
   }

   #if !defined(BOOST_INTERPROCESS_DOXYGEN_INVOKED)

   private:
   #if defined (BOOST_INTERPROCESS_USE_GENERIC_EMULATION)
      #undef BOOST_INTERPROCESS_USE_GENERIC_EMULATION
      ipcdetail::spin_condition m_condition;
   #elif defined(BOOST_INTERPROCESS_USE_POSIX)
      #undef BOOST_INTERPROCESS_USE_POSIX
      ipcdetail::posix_condition m_condition;
   #elif defined(BOOST_INTERPROCESS_USE_WINDOWS)
      #undef BOOST_INTERPROCESS_USE_WINDOWS
      ipcdetail::windows_condition m_condition;
   #else
      #error "Unknown platform for interprocess_mutex"
   #endif
   #endif   //#ifndef BOOST_INTERPROCESS_DOXYGEN_INVOKED
};

}  //namespace interprocess
}  // namespace lslboost

#include <lslboost/interprocess/detail/config_end.hpp>

#endif // BOOST_INTERPROCESS_CONDITION_HPP