#pragma once
// ***************************************************************
//  zce_child_process  -  a program this process starts, reads and stops, with what it starts
// ***************************************************************
// zce::Process is a HostVM worker: the host's own program, talking to its host through a pipe of
// zdp packets and restarted by it. A ChildProcess is any other program a process runs and waits
// for: a task a module starts per run (ymwang78/KeelTask), a solver run headless. libuv cannot be
// used for that from outside libzce: a module links the shared libzce, which exports no libuv.
// ***************************************************************
#include <zce/zce_config.h>
#include <zce/zce_object.h>
#include <zce/zce_object_counter.h>

#include <csignal>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace zce {

class Reactor;
class TaskQueue;

/**
 * @brief A program this process starts as a child and watches on a Reactor, together with the
 * processes the child starts in turn (its tree).
 *
 * - argv reaches the program as it is: no shell parses it, so no argument is ever taken for shell
 *   syntax. A program name without a directory is looked up in PATH. Strings are UTF-8.
 * - The child inherits this process's environment with Options::env set over it, and starts in
 *   Options::cwd. Its stdin is the null device; its stdout and stderr are pipes read here.
 * - kill() reaches the whole tree. On POSIX the child leads a new session, and so a process group
 *   of its own (setsid()), which kill() signals. On Windows the child is put in a job object of its
 *   own, which kill() terminates; a process it starts in its first microseconds, before start()
 *   has put it in the job, is not in it. A process that leaves the group (a daemon calling
 *   setsid()) or the job (CREATE_BREAKAWAY_FROM_JOB) is out of reach.
 * - When the child exits, whatever it left running in its tree is killed. The exit is delivered
 *   after the output: once both pipes are closed, or Options::exit_drain_msec after the exit when
 *   a process out of reach still holds them.
 * - When this process dies, on Windows the job goes with it and the tree is killed. On POSIX
 *   nothing kills the child: a program that must not outlive this process watches for it itself
 *   (prctl(PR_SET_PDEATHSIG), as HostVM workers do), since libuv runs no code between fork() and
 *   exec().
 * - The callbacks run on the Reactor's thread or, when a TaskQueue is given, are delegated to it
 *   in the order they come. The exit callback is the last one. Dropping the last reference to the
 *   ChildProcess kills the tree, and no callback starts after that.
 *
 * start() and kill() may be called from any thread. Called from another thread, they run on the
 * Reactor's thread and wait for it there, so they must not be called from a thread the Reactor's
 * thread waits for. The destructor does not wait.
 */
class ZCE_API ChildProcess : public zce::Object {
    ZCE_OBJECT_DECLARE;
    struct Impl;
    zce::SmartPtr<Impl> impl_;

  public:
    struct Options {
        /// The program and its arguments. argv[0] is a path, or a name looked up in PATH.
        std::vector<std::string> argv;
        /// Variables set over the inherited environment, as name and value. Names cannot be
        /// empty or contain '='; on Windows they are not case sensitive.
        std::vector<std::pair<std::string, std::string>> env;
        /// The working directory; empty for this process's.
        std::string cwd;
        /// How long, in milliseconds, output is still waited for after the exit, while a process
        /// out of the tree's reach holds the pipes open. Then they are closed.
        unsigned exit_drain_msec = 1000;
    };

    /// The pipe output came from.
    enum Stream { STREAM_STDOUT = 1, STREAM_STDERR = 2 };

    /// Output as it arrives, in pieces of any size. stream is a Stream.
    using OutputCallback = std::function<void(int stream, const char* data, size_t len)>;

    /// The exit: the exit status, and the signal that ended the child, 0 if none. Windows has no
    /// signals: there term_signal is the signal kill() was called with, when the child ended
    /// after that call, and the exit status is that signal too.
    using ExitCallback = std::function<void(int64_t exit_status, int term_signal)>;

    /// syncque, when given, runs the callbacks instead of the Reactor's thread.
    explicit ChildProcess(const zce::SmartPtr<Reactor>& reactor,
                          const zce::SmartPtr<TaskQueue>& syncque = zce::SmartPtr<TaskQueue>());

    /// Kills the tree if the child still runs, without waiting for the Reactor's thread, even past
    /// its delegate cap. No callback starts after it. When the Reactor has stopped, the handles
    /// are closed here and the Reactor is not held any more; a POSIX child is also reaped.
    ~ChildProcess();

    ChildProcess(const ChildProcess&) = delete;
    ChildProcess& operator=(const ChildProcess&) = delete;

    /**
     * @brief Starts the program. A ChildProcess starts once: any later call returns -1, after a
     * failed one too. To try another program, use another ChildProcess.
     * @return 0, or a negative error: libuv's (UV_ENOENT when there is no such program, UV_EINVAL
     * for an empty argv or a bad variable name), or -1 when start() was called before, or the
     * Reactor does not run or refused the call.
     */
    int start(const Options& options, const OutputCallback& on_output, const ExitCallback& on_exit);

    /**
     * @brief Sends signum to the tree: to its process group on POSIX. On Windows any signum
     * terminates the job's processes at once, with signum as their exit status.
     * @return 0 when sent; a negative error when the child does not run (UV_ESRCH) or the signal
     * could not be sent.
     */
    int kill(int signum = SIGTERM);

    /// The child's pid while it runs; 0 before start() and once its exit is delivered.
    int pid() const;

    /// Started, and the exit not delivered yet.
    bool isRunning() const;
};

using ChildProcessPtr = zce::SmartPtr<ChildProcess>;

}  // namespace zce
