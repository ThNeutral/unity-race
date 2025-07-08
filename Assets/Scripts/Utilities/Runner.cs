using System;
using System.Threading;
using System.Threading.Tasks;

namespace Utilities
{
    public class Runner
    {
        private readonly Func<bool> action;
        private int timeoutMilliseconds = Timeout.Infinite;
        private int retryCount = 0;

        public Runner(Func<bool> action)
        {
            this.action = action ?? throw new ArgumentNullException(nameof(action));
        }

        public Runner WithTimeout(int milliseconds)
        {
            timeoutMilliseconds = milliseconds;
            return this;
        }

        public Runner WithRetry(int times)
        {
            retryCount = times;
            return this;
        }

        public bool Run()
        {
            int attempts = 0;

            do
            {
                var task = Task.Run(action);
                if (!task.Wait(timeoutMilliseconds) || !task.Result)
                {
                    attempts += 1;
                    continue;
                }
                return true;
            } while (attempts < retryCount);

            return false;
        }
    }
}