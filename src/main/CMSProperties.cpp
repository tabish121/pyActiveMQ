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

#include <boost/python/class.hpp>
#include <boost/python/manage_new_object.hpp>

#include <cms/CMSProperties.h>

namespace py = boost::python;

using cms::CMSProperties;

void export_CMSProperties()
{
    const char* (CMSProperties::*CMSProperties_getProperty1)(const std::string&) const = &CMSProperties::getProperty;
    std::string (CMSProperties::*CMSProperties_getProperty2)(const std::string&, const std::string&) const = &CMSProperties::getProperty;

    py::class_<CMSProperties, boost::noncopyable>("CMSProperties", py::no_init)
        .def("__repr__", &CMSProperties::toString)
        .def("isEmpty", &CMSProperties::isEmpty)
        .def("getProperty", CMSProperties_getProperty1, py::arg("name"))
        .def("getProperty", CMSProperties_getProperty2, (py::arg("name"), py::arg("defaultValue")))
        .def("hasProperty", &CMSProperties::hasProperty, py::arg("name"))
        .def("setProperty", &CMSProperties::setProperty, (py::arg("name"), py::arg("value")))
        .def("hasProperty", &CMSProperties::hasProperty, py::arg("name"))
        .def("remove", &CMSProperties::remove, py::arg("name"))
#if 0
        .def("toArray", &CMSProperties::toArray)
#endif
        .def("copy", &CMSProperties::copy, py::arg("source"))
        .def("clone", &CMSProperties::clone, py::return_value_policy<py::manage_new_object>())
        .def("clear", &CMSProperties::clear)
        ;
}
