#pragma once

#include "ReducerMapping/Instance.hpp"
#include "Selector/Instance.hpp"

#define BURP_REDUX_SUB_STATE(NAME, FIELD, CSTATE, CPARAMS, STATE, PARAMS)\
  const STATE * getState(const CSTATE * state) {\
    return state->NAME;\
  }\
  void setState(CSTATE & combinedState, const STATE * state) {\
    combinedState.NAME = state;\
  }\
  PARAMS & getParams(CPARAMS & params) {\
    return params.NAME;\
  }\
  const PARAMS & getConstParams(const CPARAMS & params) {\
    return params.NAME;\
  }\
  using ReducerMapping = BurpRedux::ReducerMapping::Instance<CSTATE, CPARAMS, STATE, PARAMS, FIELD, getState, setState, getParams, getConstParams>;\
  template <size_t subscriberCount>\
  using Selector = BurpRedux::Selector::Instance<CSTATE, STATE, getState, subscriberCount>;
