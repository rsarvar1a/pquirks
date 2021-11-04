#!/bin/bash

if [[ -e "src/rules/$1" ]]; then
  echo "There is already a rule at 'src/rules/$1/'."
  exit 1
fi

# Create the rule stub.

mkdir src/rules/$1

cat <<EOF > "src/rules/$1/$1.h"

#ifndef $1_H_
#define $1_H_

#include <Rule.h>
#include <String.h>

DERIVE_BASE($1);

#endif

EOF

cat <<EOF > "src/rules/$1/$1.cpp"

#include "$1.h"

std::string $1::description() const
{
  return "description";
}

void $1::initialize(History & history) const
{
  return;
}

std::ostream & $1::print_state(std::ostream & out, const History & history) const
{
  return Base::print_state(out, history);
}

bool $1::test(const std::string & word, const History & history) const
{
  return true;
}

EOF

out=src/rules/Rules.h
# Update the header.

cat << EOF > $out

#ifndef PQ_RULES_H_
#define PQ_RULES_H_

#include <Macro.h>
#include <Rule.h>

EOF

for ruledir in `exa -D src/rules`; do
  cat << EOF >> $out
#include inc($ruledir)
EOF
done

cat << EOF >> $out

inline void initialize_rules()
{
EOF

for ruledir in `exa -D src/rules`; do
  cat << EOF >> $out
  ${ruledir}::instance();
EOF
done

cat << EOF >> $out
}

#endif

EOF

