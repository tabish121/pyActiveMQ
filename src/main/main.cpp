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

#include <boost/python.hpp>
#include <activemq/core/ActiveMQConnectionFactory.h>

using namespace boost::python;
using activemq::core::ActiveMQConnectionFactory;

struct World
{
    void set(std::string msg)
    {
        this->msg = msg;
    }

    std::string greet()
    {
        return msg;
    }

    std::string msg;
};

BOOST_PYTHON_MODULE(pyactivemq)
{
    class_<World>("World")
        .def("greet", &World::greet)
        .def("set", &World::set)
        ;
    class_<ActiveMQConnectionFactory>("ActiveMQConnectionFactory")
        .def(init<const std::string&>())
        .def(init<const std::string&, const std::string&>())
        .def(init<const std::string&, const std::string&, const std::string&>())
        .def(init<const std::string&, const std::string&, const std::string&, const std::string&>())
#if 0
        .def("createConnection", &ActiveMQConnectionFactory::createConnection)
#endif
        ;
}
