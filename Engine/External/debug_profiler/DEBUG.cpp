

#include <DEBUG.hpp>
#if DEBUG_PROFILE
#include <bits/chrono.h>

#include <Global.hpp>
#include <OS/OS.hpp>
#include <chrono>
#include <cstring>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>

// Debug_profiler *debug = new Debug_profiler();
void Debug_profiler::print_to_console(std::string_view time,
                                      std::string_view file,
                                      int              line,
                                      std::string_view func,
                                      std::string_view type_id = "")
{
    // FUNCTION_PROFILE
    //
    //
    // function
    OS::TERMINAL::OS_console_out(
        " \n \n[ FUNCTION ] ->  \t  ", SkyBlue, BLACK, Bold);
    OS::TERMINAL::OS_console_out(func, VividViolet, Lavender, Bold);
    if (type_id.length() != 0)
    {
        OS::TERMINAL::OS_console_out(" \n ARGS ->  \t", Peach, BLACK, Bold);
        OS::TERMINAL::OS_console_out(
            std::format({"\n {}"}, type_id), Plum, BLACK, Bold);
    }

    // line number
    OS::TERMINAL::OS_console_out("\n [LINE]->  ", Rose, BLACK, Bold);
    OS::TERMINAL::OS_console_out(
        std::to_string(line), GoldenYellow, Rose, Italic);
    // time of reaching
    OS::TERMINAL::OS_console_out("\n [ TIME ]-> ", RED, BLACK, Bold);
    OS::TERMINAL::OS_console_out(time, RED, BLUE, Bold);
    // file of
    OS::TERMINAL::OS_console_out("\n [ FILE ]", GREEN, BLACK, Bold);
    OS::TERMINAL::OS_console_out(file, Teal, BLACK, Bold);
}

void Debug_profiler::get_passed_time()
{
    auto timediff = std::chrono::high_resolution_clock::now() - start_Time;
    auto fraction =
        std::chrono::duration_cast<std::chrono::nanoseconds>(timediff);
    set_time(fraction);
}

Debug_profiler::Debug_profiler()
{
    // this is a init  function for tracy

    // while (!TracyIsConnected)
    //     std::this_thread::sleep_for(std::chrono::milliseconds(10));

    //////
    std::filesystem::path              fs = std::filesystem::current_path();
    std::vector<std::filesystem::path> txtfilelist;

    // listing all txt files
    for (const auto &entry : std::filesystem::directory_iterator(fs))
    {
        if (entry.path().extension() == ".txt")
        {
            txtfilelist.push_back(entry.path());
            std::cout << entry.path().stem() << std::endl;
        }
    }
    // looking for right log file
    if (txtfilelist.size())
    {
        int         i = 1;
        std::string file_name{"LOG"};
        for (const auto &path : txtfilelist)
        {
            if (path.stem() == file_name)
            {
                std::cout << "file " << file_name << " found" << std::endl;
                file_name = std::format("LOG_{}", i++);
            }
        }
        file_name = file_name + ".txt";
        logout_txt.open(file_name.c_str(), std::fstream::out);

        std::cout << "file " << file_name << " created" << std::endl;
    }
    else
    {
        logout_txt.open("LOG.txt");
    }
    logout_txt << "STARTING LOG" << std::flush;
    start_Time = std::chrono::high_resolution_clock::now();
}
void Debug_profiler::set_time(std::chrono::nanoseconds fraction)
{
    using namespace std::chrono_literals;
    min = std::chrono::floor<std::chrono::minutes>(fraction).count();

    sec = (std::chrono::floor<std::chrono::seconds>(fraction) % 60s).count();

    milli = (std::chrono::floor<std::chrono::milliseconds>(fraction) % 60ms)
                .count();
    micro = (std::chrono::floor<std::chrono::microseconds>(fraction) % 60us)
                .count();

    nano =
        (std::chrono::floor<std::chrono::nanoseconds>(fraction) % 60ns).count();
}

void Debug_profiler::log(std::string_view file,
                         int              line,
                         const char      *func,
                         std::string      type_info = "")
{
    get_passed_time();

    std::string file_output = std::format(
        "Time: {}min {}s {}ms {}us {}ns Function {}:  {} Line: {} File: {}  \n",
        min,
        sec,
        milli,
        micro,
        nano,
        func,
        type_info,
        line,
        file);
    print_to_console(
        std::format(
            "Time: {}min {}s {}ms {}us {}ns \n", min, sec, milli, micro, nano),
        file,
        line,
        func,
        type_info);
    // std::cout <<

    // std::cout << console_output.c_str() << std::endl;

    if ((lastindex + sizeof(file_output.c_str())) >= 10248)
        logout_txt << file_buff;
    else
    {
        std::memcpy(
            &file_buff[lastindex], file_output.c_str(), file_output.size());
        lastindex += file_output.size();
    }
}
void Debug_profiler::log(std::string_view file, int line, const char *func)

{
    get_passed_time();

    std::string file_output = std::format(
        "Time: {}min {}s {}ms {}us {}ns Function {}: Line: {} File: {}  \n",
        min,
        sec,
        milli,
        micro,
        nano,
        func,
        line,
        file);
    print_to_console(
        std::format(
            "Time: {}min {}s {}ms {}us {}ns \n", min, sec, milli, micro, nano),
        file,
        line,
        func);

    // std::cout <<

    // std::cout << console_output.c_str() << std::endl;

    if ((lastindex + sizeof(file_output.c_str())) >= 10248)
        logout_txt << file_buff;
    else
    {
        std::memcpy(
            &file_buff[lastindex], file_output.c_str(), file_output.size());
        lastindex += file_output.size();
    }
}
Debug_profiler::~Debug_profiler()
{
    logout_txt << (const char *)file_buff;
    logout_txt << "CLOSING LOG \n";
    std::cout << " CLOSING LOG \n ";
    logout_txt.close();
    // this is a delay function
    {
        auto i = 999999999;
        while (i != 0)
            --i;
    }

    //
    //
    //////
}

// template <typename... Args>
// std::string Debug_profiler::type_arg(const Args &...args)
// {
//     std::ostringstream ss;
//     ((ss << "\t " << args << "\t"), ...);
//     return ss.str();
// }

void Debug_profiler::console_log(int         Line,
                                 const char *function,
                                 const char *file,
                                 std::string log)
{
    OS::TERMINAL::OS_console_out("\n AT LINE :- " + std::to_string(Line) +
                                     " \t" + function + "\t" + file + "\t \n",
                                 RED,
                                 YELLOW,
                                 Bold);
    OS::TERMINAL::OS_console_out(std::format("\t{}\n", log), RED, BLACK, Bold);
}

// function_timer::function_timer() {
//
// }
//
// function_timer::~function_timer() {}
//

#endif  // DEBU_PROFILE
