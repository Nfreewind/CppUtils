#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
namespace cmdopt = boost::program_options;
namespace fs = boost::filesystem;

//fs::create_directories(path p);
/* DirectoryTraversing */
int DirectoryTraversing(const string dir, vector<string>& files)
{
	fs::path path(dir);
	if (!fs::exists(path))
	{
		cout << path << " not existed!!!" << endl;
		return -1;
	}

	fs::directory_iterator end_iter;
	for (fs::directory_iterator iter(path); iter != end_iter; ++iter)
	{
		if (fs::is_regular_file(iter->status()))
		{
			files.push_back(iter->path().string());
		}

		if (fs::is_directory(iter->status()))
		{
			DirectoryTraversing(iter->path().string(), files);
		}
	}

	return files.size();
}

void CommandParse()
{
	string strTestPath;
	string strTestReport;
	int API_COUNT = 0x4000 - 1;
	string report = "report";
    int dwBlocking;
	string strTestConfig;
	int interface_type;
    cmdopt::options_description desc("TestBench options");
    
    desc.add_options()
        ("help",       "produce help message")
        ("config_file,f", cmdopt::value<string>(),			"the basic configuration of the engine to test")
        ("testcase,c", cmdopt::value<string>(),			"the path containing all the testcases")
		("interface_type,t", cmdopt::value<int>(&interface_type)->default_value(API_COUNT, "choose api, default:all api \n \t\t 1:\tface_detect_extract \n \t\t 2:\tface_feature_verify \n \t\t 4:\ttask_surveillance \n \t\t 8:\ttask_snaper \n \t\t 16:\tiface_engin_ioctrl \n \t\t 32:\timage_detect_extract \n \t\t 64:\tmulti_face_verify_url \n \t\t 128:\tmulti_face_verify_bin \n \t\t 256:\tmulti_face_compare_bin \n \t\t 512:\tmulti_image_import \n \t\t 1024:\tface_detect_rect \n \t\t 2048:\tiface_add_camera \n \t\t 4096:\tquery_prgress_rate \n \t\t 8192:\tcapture_on_demand"))
		("testreport,r", cmdopt::value<string>(&strTestReport)->default_value(report, "the path to store test report"))
		("blocking,b", cmdopt::value<int>(&dwBlocking)->default_value(0, "the mode of thrift, 0:blocking, 1: nonblocking"));

    cmdopt::positional_options_description pdesc;
    pdesc.add("config_file", 1);
	pdesc.add("testcase", 1);

    cmdopt::variables_map optMap;

    try
    {
        cmdopt::store(cmdopt::command_line_parser(argc, argv).options(desc).positional(pdesc).run(), optMap);
        cmdopt::notify(optMap);
    }
    catch (boost::exception & be)
    {
        cout << "You Have inputed a command with wrong format, check it out!!\n";
        cout << "Usage: options_description [options]\n";
        cout << desc;

        return 0;
    }

    if (optMap.count("help"))
    {
        cout << "Usage: options_description [options]\n";
        cout << desc;
        
        return 0;
    }

    if (optMap.count("config_file"))
    {
		strTestConfig = optMap["config_file"].as<string>();
    }
    else
    {
        cout << "you must specify test engine configuration use -C option or directly in the command\n";
        return 0;
    }

	if (optMap.count("testcase"))
	{
		strTestPath = optMap["testcase"].as<string>();
	}
	else
	{
		cout << "you must specify test case path use -P option or directly in the command\n";
		return 0;
	}

	if (optMap.count("testreport"))
	{
		strTestReport = optMap["testreport"].as<string>();
		fs::path report(strTestReport);
		if (!fs::exists(report))
		{
			fs::create_directories(report);
		}
	}
	else
	{
		strTestReport = report;
		fs::path report(strTestReport);
		if (!fs::exists(report))
		{
			fs::create_directories(report);
		}
	}

	if (optMap.count("interface_type"))
	{
		interface_type = optMap["interface_type"].as<int>();
	}
	else
	{
		interface_type = API_COUNT;
	}

	if(dwBlocking == 0)
    {    
    	//todo
    }
    else
    {
       //todo
    }
    //todo
}