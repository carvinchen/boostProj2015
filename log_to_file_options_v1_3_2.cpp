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

//struct needed?

struct levelInfo{

	int levelValue;
}myLevel;

//Global variable
int lvl_value = 0;

//levelInfo myLevel; 
  
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

void setLevelValue(int lv){

	lvl_value = lv;
} 

int getLevelValue(){

	return lvl_value;
}

//Function of info will be limited only for a severity level.
void lvlMaskFilter(int i){

	logging::trivial::severity_level severityLevel = logging::trivial::trace;
	// passed to set_filter() by reference
	logging::core::get()->set_filter(logging::trivial::severity >= i);
}

void mask_log(string lvlMask){

        using namespace logging::trivial; 
	src::severity_logger< severity_level > lg; 

	string severityGrp[] = {"trace", "debug", "info", "warning", "error", "fatal"};

	logging::trivial::severity_level severityLevel = logging::trivial::trace;

	//trace, debug, info, warning, error, fatal

	for(int i = 0; i < sizeof(severityGrp); i++){

		if(severityGrp[i] == lvlMask){

			severityLevel = static_cast< logging::trivial::severity_level >(i);
			BOOST_LOG_SEV(lg, severityLevel) << "A " << lvlMask << " severity message";

			break;
		}
	}
}

void save_log_mask(string logpath, string logMask){

	if(logpath != ""){ 
		
		//std::cout << "log path: " << logpath << "\n" ;
	  
		int typpos = logpath.find_last_of('.'); 
		//check ob sec input has .log if not then check if it is a number 
	  
		if(typpos >= 0){ 
		    string logfile_str_type = logpath.substr(typpos+1); 
	  
		    if(logfile_str_type == "log"){ 
	  
		        //set log format and save log to file 
		        logFormat((char *)logpath.c_str());
		
			//printf("lvl int: %d \n", filterValue);

			//Mask function called
			mask_log(logMask);

			printf("Path : %s \n", logpath.c_str());

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

//Function sets log information from a severity level

int filter_log(string lvlMask){

	int lvl_mask;

	if(lvlMask == "trace"){

		lvl_mask = 0;
		//logging::trivial::severity_level severityLevel = logging::trivial::trace;

	}else if(lvlMask == "debug"){

		lvl_mask = 1;
		//logging::trivial::severity_level severityLevel = logging::trivial::debug;

	}else if(lvlMask == "info"){
		//printf("level choosen: %s \n", lvlMask.c_str());

		lvl_mask = 2;
		//logging::trivial::severity_level severityLevel = logging::trivial::info;

	}else if(lvlMask == "warning"){

		lvl_mask = 3;
		//logging::trivial::severity_level severityLevel = logging::trivial::warning;

	}else if(lvlMask == "error"){

		lvl_mask = 4;
		//logging::trivial::severity_level severityLevel = logging::trivial::error;

	}else if(lvlMask == "fatal"){

		lvl_mask = 5;
		//logging::trivial::severity_level severityLevel = logging::trivial::fatal;

	}else{

		lvl_mask = 6;
	}

	return lvl_mask;	
}

 
//Function for saving log file to local place 
  
void save_log(string logpath, int filterValue){ 
  
   using namespace logging::trivial; 
   src::severity_logger< severity_level > lg; 

   //int lvl_to_limit = getLevelValue();
    
    if(logpath != ""){ 

	std::cout << "log path: " << logpath << "\n" ;
  
        int typpos = logpath.find_last_of('.'); 
        //check ob sec input has .log if not then check if it is a number 
  
        if(typpos >= 0){ 
            string logfile_str_type = logpath.substr(typpos+1); 
  
            if(logfile_str_type == "log"){ 
  
                //set log format and save log to file 
                logFormat((char *)logpath.c_str());
		
		printf("lvl int: %d \n", filterValue);

		//Filter function called
		lvlMaskFilter(filterValue); 
  
                //printf("/n [time-stampel] [Log-level] [message] /n"); 
		BOOST_LOG_SEV(lg, trace) << "An trace severity message"; 
                BOOST_LOG_SEV(lg, debug) << "A debug severity message";
                BOOST_LOG_SEV(lg, info) << "An informational severity message"; 
                BOOST_LOG_SEV(lg, warning) << "A warning severity message";
		//BOOST_LOG_SEV(lg, error) << "A error severity message"; 
                BOOST_LOG_SEV(lg, logging::trivial::error) << "A error severity message";
		BOOST_LOG_SEV(lg, fatal) << "A fatal severity message"; 
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
   //create boost help options
	
   int severity_value;
   string filePath;
   string filterLevel;
   string levelMask;
  
   try 
   { 
	    options_description desc{"\nMain Options - Log File Manipulations"}; 
	    
	    desc.add_options() 
		    ("help,h", "Help screen") 
		    ("log-file,f", value<string>(), "Save a log file to local folder. \n[~options: -i | -m | -c]")		    
		    ("log-mask,m", value<string>(), "Save log information only for given severity level. \n[~options: -f | -c]")
		    ("log-filter,i", value<string>(), "Set limit to log file in order to severity level. \n[~options: -f | -c]")
		    ("log-class,c", value<string>()->notifier(testGet), "show the local folder saves log file.");

	    options_description fileOptions{"[File Options after -f]"};
	    fileOptions.add_options()
		    ("log-filter,i", value<string>(), "Set limit to log file with severity level.");

	    options_description filterOptions{"[Filter Options after -i]"};
	    filterOptions.add_options()
		    ("log-file,f", value<string>(), "Save a log file to local folder.");

	    options_description mainOption{"Main Options - Log File Manipulations"};
	    mainOption.add(desc).add(fileOptions).add(filterOptions);

	    variables_map vm;
	    store(parse_command_line(argc, argv, desc), vm);
	    //store(parse_command_line(argc, argv, mainOption), vm);  
	    notify(vm); 
	  
	    if(argc < 2 || argc > 10){ 
	  
		    std::cout << desc << '\n'; 
	    }else{
 
		if (vm.count("help")){ 
	  
		    std::cout << desc << '\n'; 
	  
		}else if (vm.count("log-file")){ 	  
		        //std::cout << "Log file path: " << vm["log-file"].as<string>() << '\n';
			filePath = vm["log-file"].as<string>();
			if(vm.count("log-filter")){
				//std::cout << "set severity level from : " << vm["log-filter"].as<string>() << '\n';
				filterLevel = vm["log-filter"].as<string>();
				//string level to int
				severity_value = filter_log(filterLevel);
				//save the log file by level
				save_log(filePath, severity_value);
 	
			}else if(vm.count("log-mask")){

				levelMask = vm["log-mask"].as<string>();
				save_log_mask(filePath, levelMask);								
			}else{

				save_log(filePath, 0);								
			}  

		}else if (vm.count("log-mask")){ 	  
		        //std::cout << "only for level : " << vm["log-mask"].as<string>() << '\n';

			levelMask = vm["log-mask"].as<string>();
			if (vm.count("log-file")){
				filePath = vm["log-file"].as<string>();
				save_log_mask(filePath, levelMask);
			}else{
				std::cout << "\nAfter -m file path and name should be reqired. \n" << desc << '\n';
			} 

		}else if (vm.count("log-filter")){ 

			filterLevel = vm["log-filter"].as<string>();
			severity_value = filter_log(filterLevel);

			if (vm.count("log-file")){
				filePath = vm["log-file"].as<string>();
				save_log(filePath, severity_value);
			}else{
				std::cout << "\nAfter -i file path and name should be reqired. \n" << desc << '\n';
			}

		}else if (vm.count("log-class")){ 
		        std::cout << "Log class: " << vm["log-class"].as<string>() << '\n'; 

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
