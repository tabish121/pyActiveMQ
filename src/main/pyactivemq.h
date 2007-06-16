/*
  Copyright 2007 Albert Strasheim <fullung@gmail.com>

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#ifndef PYACTIVEMQ_H_
#define PYACTIVEMQ_H_

#include <boost/shared_ptr.hpp>
#include <boost/python/object/pointer_holder.hpp>
#include <boost/python/object/make_ptr_instance.hpp>

struct make_owning_holder
{
    template<class T>
    static PyObject* execute(T* p)
    {
        typedef boost::shared_ptr<T> smart_pointer;
        typedef boost::python::objects::pointer_holder<smart_pointer, T> holder_t;
        smart_pointer ptr(const_cast<T*>(p));
        return boost::python::objects::make_ptr_instance<T, holder_t>::execute(ptr);
    }
};

#endif
