#pragma once

#include "log.h"
#include <exception>
#include <memory>
#include <string_view>
#include <boost/asio.hpp>
#include <cc/asio.hpp>
#include <cc/lit.hpp>
#include <strings.hpp>

static auto& g_asp = cc::AsioPool::instance();

using asio_cancelation_t = std::weak_ptr<boost::asio::cancellation_signal>;

#define CO_SPAWN_ON(ex, coname, awaitable)                                                 \
    ([&] {                                                                                 \
        auto sig = std::make_shared<boost::asio::cancellation_signal>();                   \
        boost::asio::co_spawn(                                                             \
            ex, awaitable,                                                                 \
            boost::asio::bind_cancellation_slot(sig->slot(), [sig](std::exception_ptr e) { \
                const std::string_view s = coname;                                         \
                if (!e) {                                                                  \
                    LOGI("co#{} exited.", s);                                              \
                } else {                                                                   \
                    try {                                                                  \
                        std::rethrow_exception(e);                                         \
                    } catch (const boost::system::system_error& ec) {                      \
                        LOGI("co#{} cancelled.", s);                                       \
                    } catch (std::exception & e0) {                                        \
                        LOGE("co#{} run error: {}", s, e0.what());                         \
                    } catch (...) {                                                        \
                        LOGE("co#{} run error: Unknown", s);                               \
                    }                                                                      \
                }                                                                          \
            }));                                                                           \
        return asio_cancelation_t(sig);                                                    \
    })()

#define CO_SPAWN(coname, awaitable) \
    CO_SPAWN_ON(g_asp.get_io_context(), (coname), (awaitable))

#define CO_CANCEL(c)                                        \
    do {                                                    \
        if (auto raw = c.lock()) {                          \
            raw->emit(boost::asio::cancellation_type::all); \
        }                                                   \
    } while (0)
