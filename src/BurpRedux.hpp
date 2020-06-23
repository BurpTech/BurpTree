#pragma once

#include "BurpRedux/Action.hpp"
#include "BurpRedux/CombinedReducer.hpp"
#include "BurpRedux/ReducerMapping/Instance.hpp"
#include "BurpRedux/Selector/Instance.hpp"
#include "BurpRedux/Store/Instance.hpp"

#define BURP_REDUX_SUB_STATE(NAME, CSTATE, CPARAMS, STATE, ACTION, REDUCER, SUBSCRIBER_COUNT, SUBSCRIBERS)\
  const STATE * get(const CSTATE * state) {\
    return state->NAME;\
  }\
  const void set(CPARAMS & params, const STATE * state) {\
    params.NAME = state;\
  }\
  using ReducerMapping = BurpRedux::ReducerMapping::Instance<CSTATE, CPARAMS, STATE, ACTION>;\
  ReducerMapping reducerMapping(get, set, REDUCER);\
  using Selector = BurpRedux::Selector::Instance<CSTATE, STATE, SUBSCRIBER_COUNT>;\
  Selector selector(get, Selector::Subscribers(SUBSCRIBERS));
