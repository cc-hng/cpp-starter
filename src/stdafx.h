#pragma once

#include <cc/asio.hpp>
#include <cc/json.h>
#include <cc/lit.hpp>
#include <cc/service.h>
#include <cc/signal.h>
#include <cc/singleton_provider.h>
#include <cc/sqlite3.h>
#include <cc/value.h>
#include <gsl/gsl>

#if 0
#    include <mutex>

using ConcurrentSignalProvider = cc::SingletonProvider<cc::Signal<std::recursive_mutex>>;

static auto& g_sig = ConcurrentSignalProvider::instance();
static auto& g_asp = cc::AsioPool::instance();
static auto& g_sql = cc::SingletonProvider<cc::Sqlite3pp<std::recursive_mutex>>::instance();

using condvar_t   = cc::CondVar<std::mutex>;
using semaphore_t = cc::Semaphore<std::mutex>;
#endif

static auto& g_sig = cc::SingletonProvider<cc::Signal<>>::instance();
static auto& g_svc = cc::SingletonProvider<cc::Service<>>::instance();
static auto& g_asp = cc::AsioPool::instance();
static auto& g_sql = cc::SingletonProvider<cc::Sqlite3pp<>>::instance();

using condvar_t   = cc::CondVar<>;
using semaphore_t = cc::Semaphore<>;
