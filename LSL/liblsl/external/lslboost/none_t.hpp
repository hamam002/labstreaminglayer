// Copyright (C) 2003, Fernando Luis Cacciola Carballal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.lslboost.org/LICENSE_1_0.txt)
//
// See http://www.lslboost.org/libs/optional for documentation.
//
// You are welcome to contact the author at:
//  fernando_cacciola@hotmail.com
//
#ifndef BOOST_NONE_T_17SEP2003_HPP
#define BOOST_NONE_T_17SEP2003_HPP

namespace lslboost {

namespace detail { struct none_helper{}; }

typedef int detail::none_helper::*none_t ;

} // namespace lslboost

#endif

