/*
 *          Copyright Andrey Semashev 2007 - 2013.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */

#include <fstream>
#include <string.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/algorithm/string.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

using namespace std;
using namespace boost;

//[ example_tutorial_formatters_format
void init()
{
    string sepLine(" \n ------------------------ \n ");

    typedef sinks::synchronous_sink< sinks::text_ostream_backend > text_sink;
    boost::shared_ptr< text_sink > sink = boost::make_shared< text_sink >();

    sink->locked_backend()->add_stream(
        boost::make_shared< std::ofstream >("sample3.log"));

    // This makes the sink to write log records that look like this:
    // 1: <normal> A normal severity message
    // 2: <error> An error severity message
    sink->set_formatter
    (
        expr::format("%1%: <%2%> %3%")
            % expr::attr< unsigned int >("LineID")
            % logging::trivial::severity
            % expr::smessage
    );

    logging::core::get()->add_sink(sink);
}
//]

int main(int, char*[])
{
    src::severity_logger<> lg2;

    init();

    for(int j = 0; j < 3; j++){

	    BOOST_LOG(lg2) << "---------------T-------------- \n";

	    logging::add_common_attributes();

	    using namespace logging::trivial;
	    src::severity_logger< severity_level > lg;

	    BOOST_LOG_SEV(lg, trace) << "A trace severity message";
	    BOOST_LOG_SEV(lg, debug) << "A debug severity message";
	    BOOST_LOG_SEV(lg, info) << "An informational severity message";
	    BOOST_LOG_SEV(lg, warning) << "A warning severity message";
	    BOOST_LOG_SEV(lg, error) << "An error severity message";
	    BOOST_LOG_SEV(lg, fatal) << "A fatal severity message";

    }
    
    //sink->flush();
    return 0;
}
