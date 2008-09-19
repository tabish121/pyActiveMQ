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

#include <boost/python/module.hpp>
#include <boost/python/class.hpp>
#include <boost/python/to_python_converter.hpp>
#include <boost/python/scope.hpp>
#include <boost/python/tuple.hpp>
#include <boost/python/list.hpp>
#include <boost/python/docstring_options.hpp>

#include <cms/Startable.h>
#include <cms/Stoppable.h>
#include <cms/Closeable.h>
#include <cms/DeliveryMode.h>

namespace py = boost::python;

using cms::Startable;
using cms::Stoppable;
using cms::Closeable;

template<typename T>
struct to_tuple
{
    static PyObject* convert(const T& c)
    {
        py::list result;
        for (typename T::const_iterator it = c.begin();
             it != c.end();
             ++it)
        {
            result.append(*it);
        }
        return py::incref(py::tuple(result).ptr());
    }
};

template<typename T>
struct std_vector_to_tuple
{
    std_vector_to_tuple()
    {
        py::to_python_converter<std::vector<T>, to_tuple<std::vector<T> > >();
    }
};

void export_CMSException();
void export_ConnectionFactory();
void export_ActiveMQConnectionFactory();
void export_Connection();
void export_Session();
void export_Destinations();
void export_Message();
void export_TextMessage();
void export_BytesMessage();
void export_MapMessage();
void export_ExceptionListener();
void export_MessageProducer();
void export_MessageListener();
void export_MessageConsumer();

static const char* Pyactivemq_docstring = "pyactivemq is a Python module for communicating with the "
										  "U{ActiveMQ<http://www.activemq.org>} message broker which "
										  "implements the Java Message Service specification.";
static const char* Startable_docstring = "Interface for a class that implements the C{start} method.";
static const char* Startable_start_docstring = "Starts the service.";
static const char* Stoppable_docstring = "Interface for a class that implements the C{stop} method.";
static const char* Stoppable_stop_docstring = "Stops this service.";
static const char* Closeable_docstring = "Interface for a class that implements the C{close} method.\n\n"
										 "The object is generally no longer usable after calling C{close}.";
static const char* Closeable_close_docstring = "Closes this object and deallocates the appropriate resources.";
static const char* DeliveryMode_docstring = "This is an abstract class whose purpose is to provide a container for "
											"the delivery mode enumeration for CMS messages.";

BOOST_PYTHON_MODULE(pyactivemq)
{
    PyEval_InitThreads();

	bool show_user_defined = true;
	bool show_py_signatures = false;
	bool show_cpp_signatures = false;
	py::docstring_options doc_options(show_user_defined, show_py_signatures);

    std_vector_to_tuple<std::string>();

    py::scope().attr("__version__") = "0.1.0";

	py::scope().attr("__doc__") = Pyactivemq_docstring;
					
    py::class_<Startable, boost::noncopyable>("Startable", Startable_docstring, py::no_init)
        .def("start", &Startable::start, Startable_start_docstring)
        ;

    py::class_<Stoppable, boost::noncopyable>("Stoppable", Stoppable_docstring, py::no_init)
        .def("stop", &Stoppable::stop, Stoppable_stop_docstring)
        ;

    py::class_<Closeable, boost::noncopyable>("Closeable", Closeable_docstring, py::no_init)
        .def("close", &Closeable::close, Closeable_close_docstring)
        ;

    export_CMSException();
    export_ConnectionFactory();
    export_ActiveMQConnectionFactory();
    export_Connection();
    export_Session();
    export_Destinations();
    export_Message();
    export_TextMessage();
    export_BytesMessage();
    export_MapMessage();
    export_ExceptionListener();
    export_MessageProducer();
    export_MessageListener();
    export_MessageConsumer();

    // Assign DeliveryModes to ints first to avoid issue with
    // unresolved symbols at runtime when compiling with GCC.
    py::scope s = py::class_<cms::DeliveryMode,
		boost::noncopyable>("DeliveryMode", DeliveryMode_docstring, py::no_init);
	// Enumeration values for Message Delivery Mode.
    int persistent = cms::DeliveryMode::PERSISTENT;
    s.attr("PERSISTENT") = persistent;
    int non_persistent = cms::DeliveryMode::NON_PERSISTENT;
    s.attr("NON_PERSISTENT") = non_persistent;
}
