#pragma once

#include "ReducerMapping/Instance.hpp"
#include "Selector/Instance.hpp"

#define BURP_REDUX_SUB_STATE(NAME, CSTATE, CPARAMS, STATE, REDUCER, SUBSCRIBER_COUNT, SUBSCRIBERS)\
  const STATE * get(const CSTATE * state) {\
    return state->NAME;\
  }\
  const void set(CPARAMS & params, const STATE * state) {\
    params.NAME = state;\
  }\
  using ReducerMapping = BurpRedux::ReducerMapping::Instance<CSTATE, CPARAMS, STATE>;\
  ReducerMapping reducerMapping(get, set, REDUCER);\
  using Selector = BurpRedux::Selector::Instance<CSTATE, STATE, SUBSCRIBER_COUNT>;\
  Selector selector(get, Selector::Subscribers(SUBSCRIBERS));
