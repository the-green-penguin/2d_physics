# What is this?
A simple command line argument parser for C++ programs (C++11).  
POSIX compliant (if used accordingly).  
## WARNING: This is just a hobby project of mine. I'm learning to code. So, please do not expect too much of this project!


# License:
MIT License (https://choosealicense.com/licenses/mit/)


# How to use:
1) Create an instance of the parser: `SArgParser p;`  
2) Define options and store their ID: `SArgParser::opt_id help = p.define_option('h', "help", true);`  
  Pass three arguments: short name (char), long name, is_flag (boolean).  
  If "is_flag" is set to `true`, the option does not expect arguments.  
3) Execute parser with defined options on input: `p.parse(argc, argv);`  
4) Process result:
  - `p.program_name()` returns program name (`argv[0]` as string).  
  - `p.get_program_args()` returns the arguments not associated with an option (`vector<string>`).  
  - `p.found_option(ID)` returns `true` if input contains that option at least once.  
  - `p.get_option_args(ID)` return arguments associated with given option (`vector<string>`).  

(See example/example.cpp & example/example.txt)


# Exceptions:
`parse(argc, argv)` will throw an exception (std::runtime_error) in case it receives invalid input!  
  - If an option expects arguments (is_flag == false) but no argument was found.  
  - If a long option is defined as a flag but an argument is given. (`./example --flag=error`)  
  - If an undefined option was found.  

All other exceptions are caused by bugs in `simple_arg_parser.h`...  


# POSIX:
POSIX compliant: https://www.gnu.org/software/libc/manual/html_node/Argument-Syntax.html  
... if used accordingly: It is left up to the user (programmer using this parser) to:  
  - implement the special option `-` (returned by `p.get_program_args()` if found in input).  
  - whether to restrict option names to alphanumeric characters or not.  
  - provide completion for long option names in the shell.
