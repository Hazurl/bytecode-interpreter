#include <bytec/Program/Program.hpp>
#include <bytec/Sandbox/SandBox.hpp>
#include <bytec/Interpreter/Interpreter.hpp>
#include <bytec/Instruction/OperationDefinition.hpp>
#include <bytec/Interpreter/BinRepr.hpp>

#include <bytec/Program/Example.hpp>

#include <iostream>
#include <bitset>
#include <cstdlib>
#include <ctime>

using namespace bytec;

int main() {
    //std::srand(std::time(nullptr));

    SandBox sandbox;

    {    
        Program program;
        SecuredLabel entry_point;

        // go to entry point
        program.append_instruction(Operations::JMP, entry_point);
    
        // load hello_world function
        auto hello_world = program_ex::hello_world(program);

        // entry point
        program.link(entry_point);

        // call void hello_world()
        program.append_instruction(Operations::CALL, hello_world.func);
    
        program.append_instruction(Operations::OUT, Value('\n'));
        program.append_instruction(Operations::HLT);

        sandbox.load_program(program);
    }

    std::cout << "# Start #" << std::endl;

    while(!sandbox.is_halted()) {
        auto is = sandbox.get_instruction();
        //std::cout << "\033[31m\033[1m< " << is.to_string() << " >\033[0m"; 
        Interpreter::run_next_instruction(sandbox);
        //std::cout << '\n';
        //std::cin.get();
    }

    std::cout << "# Halt #" << std::endl;

}