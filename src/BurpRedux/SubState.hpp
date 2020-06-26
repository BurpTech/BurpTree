#pragma once

#include "ReducerMapping/Instance.hpp"
#include "Selector/Instance.hpp"

#define BURP_REDUX_SUB_STATE(NAME, FIELD, CSTATE, CPARAMS, STATE)\
  const STATE * get(const CSTATE * state) {\
    return state->NAME;\
  }\
  void set(CPARAMS & params, const STATE * state) {\
    params.NAME = state;\
  }\
  using ReducerMapping = BurpRedux::ReducerMapping::Instance<CSTATE, CPARAMS, STATE, FIELD, get, set>;\
  template <size_t subscriberCount>\
  using Selector = BurpRedux::Selector::Instance<CSTATE, STATE, get, subscriberCount>;
