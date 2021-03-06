# pquirks

A party quirks rule utility.

### Installation

`pquirks` depends on `cmake`, `ninja`, `nlohmann-json` and `exa`. You can skip the `exa` dependency if you swap 
it with `ls` and filter in each loop for directories.

Build the application.

```sh
./scripts/build.sh
```

### Usage

Run the program.

```sh 
./bin/pquirks
```

To see the help menu, enter `?`.

### Creating new rules

Invoke the rule creation script. This adds a folder to `src/rules` and fills out the header and implementation
files with stubs. It then updates `src/rules/Rules.h` automatically to include the rule and register it into
the application.

```sh
./scripts/makerule.sh <rule-name>
```

You are required to only provide two implementations, which are `Rule::description()` and `Rule::test(word, history)`.
There are some string utilities built into the framework, which can be found in `src/String.h`. Optionally, if your
rule works with the history's state, you can initialize the state in `Rule::initialize(history)` and provide an output
format in `Rule::print_state(out, history)`.

For example, here is the full implementation for `SumOfOrdsLastRejected`:

`SumOfOrdsLastRejected.h`

```c++

#ifndef SumOfOrdsLastRejected_H_
#define SumOfOrdsLastRejected_H_

#include <Rule.h>
#include <String.h>

DERIVE_BASE(SumOfOrdsLastRejected_H_);

#endif

```

`SumOfOrdsLastRejected.cpp`

```c++

#include "SumOfOrdsLastRejected.h"

std::string SumOfOrdsLastRejected::description() const
{
  return "Accepts a word if the sum of its letters is equal to the sum of the letters of the last rejected word.";
}

void SumOfOrdsLastRejected::initialize(History & history) const
{
  return;
}

std::ostream & SumOfOrdsLastRejected::print_state(std::ostream & out, const History & history) const
{
  return Base::print_state(out, history);
}

bool SumOfOrdsLastRejected(const std::string & word, const History & history) const
{
  return sum_a1z26(word) == sum_a1z26(history.peek_rejected());
}

```

Once these are provided, you must rebuild the application.

### Deriving behaviour from sub-rules

You can also have a rule that derives from a previous rule, so that you can apply transformations to the mapping
provided by the base rule. For example, here is a rule that accepts all strings accepted by `SumOfOrdsLastRejected`
with 80% probability, and accepts all strings rejected by `SumOfOrdsLastRejected` with 20% probability. (Evil!)

`DecoratedRule.h`

```c++

#ifndef DecoratedRule_H_
#define DecoratedRule_H_

#include <Macro.h>
#include <Rule.h>
#include <String.h>

#include rule(SumOfOrdsLastRejected)

DERIVE_RULE(DecoratedRule, SumOfOrdsLastRejected);

#endif

```

`DecoratedRule.cpp`

```c++ 

std::string DecoratedRule::description() const
{
  return 
    "Accepts a word with 80% probability if the sum of its letters is equal to the sum of the " 
    "letters of the last rejected word, and accepts with 20% probability otherwise.";
}

void DecoratedRule::initialize(History & history) const
{
}

std::ostream & DecoratedRule::print_state(std::ostream & out, const History & history) const
{
  return Base::print_state(out, history);
}

bool DecoratedRule::test(const std::string & word, const History & history) const
{
  return Base::test(word, history)
    ? rand() % 100 < 80
    : rand() % 100 < 20;
}

```
