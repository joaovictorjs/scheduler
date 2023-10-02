# ⏱️ Scheduler
An easy to use c++ library for scheduling tasks.

## Summary
- [Installation] (#installation)
- [Example] (#example)

## Installation

- Getting the library from source:

```bash
git clone https://github.com/joaovictorjs/scheduler
```

- Setup and compile (meson required):

```bash
cd scheduler &&
meson setup --prefix=/usr --buildtype=release build &&
meson compile -C build
```

- Test the code (optional):

```
meson test -C build
```

- Finally, install the library:

``` bash
meson install -C build
```

## Example
__Purpose:__ create a software that will print each 5 seconds to console 
*"Hello, i'm a scheduled task. Now is [actual datetime]"* until *2100-01-01 00:00:00*.

- File: meson.build

``` cpp
project('scheduler-example', 'cpp')

executable(
	'scheduler-example', 
	sources: 'main.cpp', 
	dependencies: dependency('scheduler')
)
```

- File: main.cpp

``` cpp
#include <iomanip>
#include <iostream>
#include <scheduler.hpp>

void print_hello();

int main() {
	Scheduler::Task task;
	
	task.set_interval(5)
		.set_datetime_start("2000-01-01 00:00:00")
		.set_datetime_end("2100-01-01 00:00:00")
		.run(print_hello);
}

void print_hello() {
	std::time_t now {std::time(nullptr)};
	std::tm* now_tm {std::localtime(&now)};
	std::cout<<"Hello, i'm a scheduled task. Now is "<<std::put_time(now_tm, "%Y-%m-%d %H:%M:%S")<<'\n';
}
```

- Setup and compile (meson required):

``` bash
meson setup build && meson compile -C build
```

- Running: 

```
./build/scheduler-example
```