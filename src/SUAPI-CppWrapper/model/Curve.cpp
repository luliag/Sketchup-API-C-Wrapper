//
//  Curve.cpp
//
// Sketchup C++ Wrapper for C API
// MIT License
//
// Copyright (c) 2017 Tom Kaneko
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

// Macro for getting rid of unused variables commonly for assert checking
#define _unused(x) ((void)(x))

#include <algorithm>
#include <cassert>
#include <stdexcept>

#include "SUAPI-CppWrapper/model/Curve.hpp"

namespace CW {


Curve::Curve(std::vector<Edge> edges, SUCurveType curve_type):
  Curve(create_curve(edges, m_create_result), curve_type)
{
}

Curve::Curve(SUCurveRef curve,  SUCurveType curve_type):
  Entity(SUCurveToEntity(curve)),
  m_curve_type(curve_type)
{}

Curve::~Curve(){
}

SUCurveRef Curve::create_curve(std::vector<Edge>& edges, SUResult &result) {
  SUCurveRef curve_ref = SU_INVALID;
  std::vector<SUEdgeRef> refs(edges.size(), SU_INVALID);
  
  std::transform(edges.begin(), edges.end(), refs.begin(), [](const CW::Edge& edge) {return edge.ref(); });
  result = SUCurveCreateWithEdges(&curve_ref, refs.data(), refs.size());
  return curve_ref;
}


SUCurveRef Curve::ref() const {
  return SUCurveFromEntity(m_entity);
}


std::vector<Edge> Curve::get_edges() const{
  if (!(*this)) {
    throw std::logic_error("CW::Curve::get_edges(): Curve is null");
  }
  size_t num_edges = 0;
  SUResult res = SUCurveGetNumEdges(this->ref(), &num_edges);
  assert(res == SU_ERROR_NONE);
  std::vector<SUEdgeRef> ref_edges(num_edges, SU_INVALID);
  res = SUCurveGetEdges(this->ref(), num_edges, ref_edges.data(), &num_edges);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<Edge> edges(num_edges);
  std::transform(ref_edges.begin(), ref_edges.end(), edges.begin(),
  [](const SUEdgeRef& value) {
    return Edge(value);
  });
  return edges;
}


SUCurveType Curve::get_type() {
  if (!(*this)) {
    throw std::logic_error("CW::Curve::get_type(): Curve is null");
  }
  SUCurveGetType(this->ref(), &m_curve_type);
  return m_curve_type;
}
 

SUResult Curve::get_result() const {
  return m_create_result;
}


Curve::operator bool() const {
  if (m_create_result == SU_ERROR_NONE) {
    return true;
  }
  return false;
}


bool Curve::operator!() const {
  return !bool();
}

} /* namespace CW */
