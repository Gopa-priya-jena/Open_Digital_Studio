
#include <Define.hpp>
#if ODS_LINUX
#include <sys/poll.h>
#include <unistd.h>

#include <DEBUG.hpp>
#include <Global.hpp>
#include <OS/LINUX/LINUX_SHELL.hpp>
#include <OS/OS_SHELL.hpp>
#include <cerrno>
#include <cstdlib>
#include <stdexcept>
#include <string_view>

namespace OS
{
namespace SHELL
{

void Terminal(const std::string_view command,
              char                  &output,
              unsigned int           output_size)
{
    FUNCTION_PROFILE;

    assert(output_size >= 3);
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.data(), "r"),
                                                  pclose);
    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }

    while (fgets(&output, (int)output_size, pipe.get()) != nullptr)
        ;
}

OS_SHELL::OS_SHELL(SHELL_Type type)
{
    FUNCTION_PROFILE;

    if (pipe(SHELL_Linux.WRITE) == -1 || pipe(SHELL_Linux.READ) == -1 ||
        pipe(SHELL_Linux.ERROR) == -1)

    {
        LOG("PIPES creation  failed");
        throw std::runtime_error("Failed to create pipes");
    }
    else
    {
        const char *path = "bash";
        const char *argV[] = {const_cast<char *>(path), "-C", nullptr};
        posix_spawn_file_actions_t file_actions;
        posix_spawn_file_actions_init(&file_actions);

        posix_spawn_file_actions_adddup2(
            &file_actions, SHELL_Linux.WRITE[0], STDIN_FILENO);
        posix_spawn_file_actions_addclose(&file_actions, SHELL_Linux.WRITE[1]);

        posix_spawn_file_actions_adddup2(
            &file_actions, SHELL_Linux.READ[1], STDOUT_FILENO);
        posix_spawn_file_actions_addclose(&file_actions, SHELL_Linux.READ[0]);

        posix_spawn_file_actions_adddup2(
            &file_actions, SHELL_Linux.ERROR[1], STDERR_FILENO);
        posix_spawn_file_actions_addclose(&file_actions, SHELL_Linux.ERROR[0]);

        if (posix_spawnp(&SHELL_Linux.processID,
                         path,
                         &file_actions,
                         NULL,
                         static_cast<char **>(const_cast<char **>(argV)),
                         environ) == 0)
        {
            LOG("SHELLL ACTIVE")

            close(SHELL_Linux.WRITE[0]);
            close(SHELL_Linux.READ[1]);
            close(SHELL_Linux.ERROR[1]);

            // prepare th poll data structure

            SHELL_Linux.Eventpoll[0].fd = SHELL_Linux.READ[0];
            SHELL_Linux.Eventpoll[1].fd = SHELL_Linux.WRITE[1];
            SHELL_Linux.Eventpoll[2].fd = SHELL_Linux.ERROR[0];

            // events setting of Read

            SHELL_Linux.Eventpoll[0].events = POLLIN;
            SHELL_Linux.Eventpoll[0].revents = 0;

            // events setting of Write
            SHELL_Linux.Eventpoll[1].events = POLLOUT;
            SHELL_Linux.Eventpoll[1].revents = 0;

            // events setting of ERROR
            SHELL_Linux.Eventpoll[2].events = POLLIN;
            SHELL_Linux.Eventpoll[2].revents = 0;

            if (type == SHELL_Type::ASYNC)
            {
                Thread_init();
            }
        }

        posix_spawn_file_actions_destroy(&file_actions);
    }
}
void OS_SHELL::check_error()
{
    FUNCTION_PROFILE;

    char    buffer[1024];
    ssize_t bytesRead;
    OS_console_out("ERROR in shell: ", RED, BLACK, Bold);

    while ((bytesRead =
                read(SHELL_Linux.ERROR[0], buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[bytesRead] = '\0';
        OS::TERMINAL::OS_console_out(buffer, RED, BLACK, Bold);
    }
}
void OS_SHELL::update_Eventpoll()
{
    FUNCTION_PROFILE;

start_again:
    auto result = poll(SHELL_Linux.Eventpoll, 3, 0);

    if (result == -1 || errno == EAGAIN) goto start_again;
    if (result == 3 || SHELL_Linux.Eventpoll[2].revents & POLLIN) check_error();
}

void OS_SHELL::Thread_init()
{
    FUNCTION_PROFILE;

    // std::thread read(output());
}
OS_SHELL::~OS_SHELL()

{
    FUNCTION_PROFILE;

    close(SHELL_Linux.WRITE[1]);
    close(SHELL_Linux.READ[0]);
    close(SHELL_Linux.ERROR[0]);
    // Thread_close();
}
bool OS_SHELL::Get_output()
{
    FUNCTION_PROFILE;

    update_Eventpoll();

    while (SHELL_Linux.Eventpoll[0].revents != POLLIN)
        update_Eventpoll();
    {
        output_size = read(
            SHELL_Linux.READ[0], &Output_Buffer[0], OUTPUT_BUFFER_SIZE - 1);
        Output_Buffer[output_size] = '\0';
        OUTPUT = std::string_view(Output_Buffer, output_size);
    }
    update_Eventpoll();

    if (SHELL_Linux.Eventpoll[0].revents == POLLIN)
        return true;
    else
        return false;
}
bool OS_SHELL::Send_command(std::string_view command)
{
    FUNCTION_PROFILE;

    update_Eventpoll();

    while (SHELL_Linux.Eventpoll[1].revents != POLLOUT)
        update_Eventpoll();

    if (command.back() == '\n')
    {
        auto res = write(SHELL_Linux.WRITE[1], command.data(), command.size());

        return res == command.size();
    }
    else
    {
        auto last_pos = command.copy(command_buffer, 1023);
        command_buffer[last_pos] = '\0';
        auto res = write(SHELL_Linux.WRITE[1], command_buffer, last_pos + 1);
        return res == last_pos + 1;
    }
}

bool OS_SHELL::shell(std::string_view command)
{
    FUNCTION_PROFILE;

    if (Send_command(command))
    {
        return Get_output();
    }
    else
    {
        ERROR = Write_error;
        return 0;
    }
}

std::string OS_SHELL::shell_read_complete()
{
    FUNCTION_PROFILE;

    std::string result;

    if (Get_output())
    {
        result += OUTPUT.substr(0, output_size);
    }
    return result;
}

void OS_SHELL::flush_output()
{
    FUNCTION_PROFILE;

    char flush_buff[1024];

    while (read(SHELL_Linux.READ[0], &flush_buff[0], 1024))
        ;
}
std::string_view OS_SHELL::exec(std::string_view command)
{
    FUNCTION_PROFILE;

    if (shell(command))
    {
        ALERT(ERROR);
        flush_output();
    }
    return OUTPUT;

}  // namespace SHELL

}  // namespace SHELL
}  // namespace OS
#endif
