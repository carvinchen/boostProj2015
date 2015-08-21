#include <fstream>   
#include <boost/shared_ptr.hpp>   
#include <boost/make_shared.hpp>   
#include <boost/log/core.hpp> 
#include <boost/log/common.hpp> 
#include <boost/log/trivial.hpp>   
#include <boost/log/expressions.hpp>   
#include <boost/log/sinks/sync_frontend.hpp>   
#include <boost/log/sinks/text_ostream_backend.hpp>   
#include <boost/log/sources/severity_logger.hpp>   
#include <boost/log/sources/record_ostream.hpp>   
#include <boost/log/utility/setup/common_attributes.hpp> 
#include <boost/log/utility/setup/file.hpp> 
#include <boost/log/support/date_time.hpp>   
#include <boost/thread/thread.hpp>   
#include <boost/log/attributes/scoped_attribute.hpp> 
#include <boost/program_options.hpp> 
#include <stdio.h>
#include <iostream> 
#include <string.h> 
  
//using namespace boost::algorithm; 
using namespace std; 
using namespace boost::program_options; 
//using namespace boost::log; 
  
namespace logging = boost::log; 
namespace sinks = boost::log::sinks; 
namespace expr = boost::log::expressions; 
namespace src = boost::log::sources; 
namespace triv = logging::trivial;
  
/* 
void init() 
{ 
    logging::add_file_log("sample.log"); 
    
    logging::core::get()->set_filter 
    ( 
        logging::trivial::severity >= logging::trivial::info 
    ); 
  
  
}*/ 
  
void logFormat(char logFile[]){ 
    
    typedef sinks::synchronous_sink< sinks::text_ostream_backend > text_sink; 
    boost::shared_ptr< text_sink > sink = boost::make_shared< text_sink >(); 
  
    sink->locked_backend()->add_stream(boost::make_shared< std::ofstream >(logFile)); 
    sink->set_formatter (   
           expr::format("[%1%] - [%2%] - [%3%]")   
           % expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")   
           % logging::trivial::severity 
           % expr::smessage   
        );   
  
    logging::core::get()->add_sink(sink); 
    //Ohne add_common_attributes(); wird das Datum nicht angezeigt...?? 
    logging::add_common_attributes(); 
  
} 
  
void saveLog(char logFile[]){ 
  
    logging::add_file_log(logFile); 
  
} 
  
void testGet(string logpath){ 
  
    std::cout << logpath << " \n"; 
  
} 
  
//Function for saving log file to local place 
  
void save_log(string logpath){ 
  
   using namespace logging::trivial; 
   src::severity_logger< severity_level > lg; 
    
    if(logpath != ""){ 

	std::cout << "log path: " << logpath << "\n" ;
  
        int typpos = logpath.find_last_of('.'); 
        //check ob sec input has .log if not then check if it is a number 
  
        if(typpos >= 0){ 
            string logfile_str_type = logpath.substr(typpos+1); 
  
            if(logfile_str_type == "log"){ 
  
                //set log format and save log to file 
                logFormat((char *)logpath.c_str()); 
  
                //printf("/n [time-stampel] [Log-level] [message] /n"); 
                BOOST_LOG_SEV(lg, info) << "An informational severity message"; 
                BOOST_LOG_SEV(lg, warning) << "A warning severity message";
		//BOOST_LOG_SEV(lg, error) << "A error severity message"; 
                BOOST_LOG_SEV(lg, logging::trivial::error) << "A error severity message"; 
                //sink->flush(); 
  
            }else{ 
                printf("the typ of a log file should be *.log \n"); 
                //cout << "the typ of a log file should be *.log \n"; 
            } 
        }else{ 
  
            printf("the file name should be like path/to/file.log \n"); 
            //cout << "the file name should be like path/to/file.log \n";   
        }  
    }else{ 
  
        printf("the input for a log file should be like path/to/file.log \n");   
        //cout << "the input for a log file should be like path/to/file.log \n"; 
    } 
  
} 
  
  
int main(int argc, const char *argv[]) 
{ 
    //init();  
   //using namespace logging::trivial; 
   //src::severity_logger< severity_level > lg; 
  
   //create boost help options 
  
   try 
   { 
  
	    options_description desc{"Options"}; 
	  
	    desc.add_options() 
		    ("help,h", "Help screen") 
		    ("log-file", value<string>()->notifier(save_log), "Save a log file to local folder.")
		    ("log-class", value<string>()->notifier(testGet), "show the local folder saves log file.");
 
	  
	  
	    variables_map vm; 
	    store(parse_command_line(argc, argv, desc), vm); 
	    notify(vm); 
	  
	    if(argc < 2 || argc > 3){ 
	  
		    std::cout << desc << '\n'; 
	    }else{
 
		if (vm.count("help")){ 
	  
		    std::cout << desc << '\n'; 
	  
		}else if (vm.count("log-file")){ 
	  
		        std::cout << "Log file path: " << vm["log-file"].as<string>() << '\n'; 
		}else if (vm.count("log-class")){ 
	  
		        std::cout << "Log file: " << vm["log-class"].as<string>() << '\n'; 
		}else{

			std::cout << desc << '\n';
		}
	  
	    } 
    
   }catch (const error &ex) 
   { 
        std::cerr << ex.what() << '\n'; 
   }
    return 0; 

}
