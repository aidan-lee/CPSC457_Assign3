#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// A struct that holds information for each process.  The information includes
// the ID of the process, its arrival time, the burst time, the burst time
// remaining, the time the process finishes executing, the state of the process,
// the time slice it has used, and its waiting time.
struct process_data
{
  int id;
  int arrival;
  int burst_length;
  int burst_remaining;
  int finish_time;
  string state;
  int slice;
  float wait;
};

int finished_processes = 0;

// A function that accesses the time and the state of all processes create a string
// to print to the console.
string stringMaker(int i, int counter, process_data PD[])
{
  string temp = "";
  temp += to_string(i);
  if (i < 10)
  {
    temp += "     ";
  }
  else if (i < 1000)
  {
    temp += "    ";
  }
  else if (i < 10000)
  {
    temp += "   ";
  }
  for (int j = 0; j < counter; j++)
  {
    temp += PD[j].state;
    temp += "    ";
  }
  temp += "\n";
  return temp;
}

// A function that prints the strings stored in 'result' to the console.
void printTable(int counter, vector<string> result)
{
  printf("Time  ");
  for (int i = 0; i < counter; i++)
  {
    printf("P%d   ", i);
  }
  printf("\n––––");
  for (int i = 0; i < counter; i++)
  {
    printf("––––––");
  }
  printf("\n");

  for (int i = 0; i < result.size(); i++)
  {
    printf("%s", result[i].c_str());
  }
  printf("––––");
  for (int i = 0; i < counter; i++)
  {
    printf("––––––");
  }
  printf("\n");

}

// A function that calculates the waiting times for each process and the
// average waiting time, and prints them to the console.
void printStats(int counter, process_data PD[])
{
  // Calculating and printing waiting times for each process
  for (int i = 0; i < counter; i++)
  {
    int turnaround = PD[i].finish_time - PD[i].arrival;
    float waiting = turnaround - PD[i].burst_length;
    PD[i].wait = waiting;
    printf("P%d waited %.3f seconds.\n", PD[i].id, waiting);
  }

  // Calculating and printing the average waiting time
  float sum = 0;
  for (int i = 0; i < counter; i++)
  {
    sum += PD[i].wait;
  }
  float average = sum / (double)counter;
  printf("Average waiting time = %.3f seconds.\n", average);
}

int main(int argc, char* const argv[])
{
  string filename;
  string algorithm;
  int burst;

  // Parsing command line arguements and handling invalid usage
  if (argc != 3 && argc != 4)
  {
    printf("Please enter the name of the configuration file, the algorithm, and the burst time, if needed.\n");
    exit(-1);
  }

  filename = argv[1];
  algorithm = argv[2];

  if (algorithm != "SJF" && algorithm != "sjf" && algorithm != "RR" && algorithm != "rr")
  {
    printf("No valid scheduling algorithm entered.  Please choose SJF or RR.\n");
    exit(-1);
  }

  if ((algorithm == "SJF" || algorithm == "sjf") && argc == 4)
  {
    printf("The SJF scheduling algorithm does not requre a burst time.\n");
    exit(-1);
  }

  if ((algorithm == "RR" || algorithm == "rr") && argc != 4)
  {
    printf("The RR scheduling algorithm requires a burst time.\n");
    exit(-1);
  }

  // Opening the file specified on the command line
  ifstream infile;
  infile.open(filename.c_str());

  // Reading the file to determine the amount of processes in the file
  string line;
  int counter = 0;

  while (getline(infile, line))
  {
    counter++;
  }

  // Initializing an array large enough to hold the processes from the file
  process_data PD [counter];

  // Preparing the input file to be read again to store process information
  infile.clear();
  infile.seekg(0, ios::beg);

  // Reading the input file again and storing the numbers into PD
  int arr;
  int b_time;
  for (int i = 0; i < counter; i++)
  {
    infile >> arr;
    infile >> b_time;
    PD[i].id = i;
    PD[i].arrival = arr;
    PD[i].burst_length = b_time;
    PD[i].burst_remaining = b_time;
    PD[i].slice = 0;
  }

  infile.close();

  // Initializing a ready queue
  vector<process_data*> queue;
  // Initializing a vector to hold the strings of processes states for
  // printing to the console later.
  vector<string> result;

  if (algorithm == "SJF" || algorithm == "sjf")
  {
    int running = 1;
    string running_process = "NOTHING";
    int i = 0;
    while(running)
    {
      // Adding processes to the ready queue as they arrive
      for (int j = 0; j < counter; j++)
      {
        if (i == PD[j].arrival)
          queue.push_back(&PD[j]);
      }

      // If no process is running, sort ready queue by burst length using
      // bubble sort
      if (running_process == "NOTHING")
      {
        for (int j = 0; j < queue.size(); j++)
        {
          for (int k = queue.size() - 1; k > j; k--)
          {
            if (queue[k]->burst_length < queue[k - 1]->burst_length)
            {
              process_data* temp = queue[k - 1];
              queue[k - 1] = queue[k];
              queue[k] = temp;
            }
          }
        }
      }

      // If there is a process, begin running it
      if (!queue.empty())
      {
        // If no process is currently running, run one.
        if (running_process == "NOTHING")
        {
          running_process = to_string(queue.at(0)->id);
        }

        // Setting state to running and decrementing burst time
        queue[0]->state = ".";
        queue[0]->burst_remaining = queue[0]->burst_remaining - 1;

        // If a process is not running, set its state to ready.
        for (int j = 0; j < queue.size(); j++)
        {
          if (running_process != to_string(queue[j]->id))
            queue[j]->state = "+";
        }

        // Storing time and process state information into a string
        string temp = stringMaker(i, counter, PD);
        result.push_back(temp);

        // Removing finished processes from the ready queue
        if (queue[0]->burst_remaining == 0)
        {
          finished_processes++;
          queue[0]->finish_time = i + 1;
          queue[0]->state = " ";
          queue.erase(queue.begin());
          running_process = "NOTHING";

          // If the ready queue is empty, all processes have been completed
          // and the simulation should end.
          if (queue.empty() && (finished_processes == counter))
            running = 0;
        }
      }
      // This else block handles the situation in which the CPU is sitting
      // idle before processes arrive.
      else
      {
        string temp = to_string(i);
        temp += "\n";
        result.push_back(temp);
      }
      i++;
    }

  }

  if (algorithm == "RR" || algorithm == "rr")
  {
    burst = atoi(argv[3]);
    int running = 1;
    string running_process = "NOTHING";
    int i = 0;
    int running_index = 0;
    while (running)
    {
      // Adding processes to the ready queue as they arrive
      for (int j = 0; j < counter; j++)
      {
        if (i == PD[j].arrival)
        {
          queue.push_back(&PD[j]);
        }
      }

      // If there is a process, run it
      if (!queue.empty())
      {
        // If no process is being run, run the first process in the ready queue
        if (running_process == "NOTHING")
        {
          running_process = to_string(queue[running_index]->id);
        }

        // Setting state to running, reducing burst time remaining, and
        // incrementing time slice used.
        queue[running_index]->state = ".";
        queue[running_index]->burst_remaining = queue[running_index]->burst_remaining - 1;
        queue[running_index]->slice = queue[running_index]->slice + 1;

        // If a process is not running, set its state to ready.
        for (int j = 0; j < queue.size(); j++)
        {
          if (running_process != to_string(queue[j]->id))
            queue[j]->state = "+";
        }

        // Storing time and process state information into a string
        string temp = stringMaker(i, counter, PD);
        result.push_back(temp);

        // If a process has finished running, remove it from the ready queue
        if (queue[running_index]->burst_remaining == 0)
        {
          finished_processes++;
          int temp_finish = i + 1;
          queue[running_index]->finish_time = temp_finish;
          queue[running_index]->state = " ";
          queue.erase(queue.begin());
          running_process = "NOTHING";

          // If the ready queue is empty, all processes have finished runnning,
          // and the simulation should end
          if (queue.empty() && (finished_processes == counter))
            running = 0;
        }
        // If a process has exhausted its time slice, it should be removed
        // from the beginning of the ready queue and moved to the end
        else if (queue[running_index]->slice == burst)
        {
          queue[running_index]->slice = 0;
          process_data* tempPD = queue[running_index];
          queue.erase(queue.begin());
          queue.push_back(tempPD);
          running_process = to_string(queue[running_index]->id);
        }
      }
      // This else block handles the situation in which the CPU is sitting
      // idle before processes arrive.
      else
      {
        string temp = to_string(i);
        temp += "\n";
        result.push_back(temp);
      }
      i++;
    }
  }

  // Printing the table to the console
  printTable(counter, result);
  // Printing the waiting times per process and average waiting time
  printStats(counter, PD);

  return 0;
}
