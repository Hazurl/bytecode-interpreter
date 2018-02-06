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
#include <chrono>

using namespace bytec;

int main() {
    std::srand(std::time(nullptr));

    SandBox sandbox;

    {    
        Program program;
        Label entry_point;

        // go to entry point
        program.append_instruction(Operations::JMP, entry_point);
    
        // load hello_world function
        auto hello_world = program_ex::hello_world(program);
        auto printer = program_ex::print_number(program);
        auto lerp = program_ex::lerp(program);
        auto sort = program_ex::sort(program);

        // entry point
        program.link(entry_point);
/*
        // call void hello_world()
        program.append_instruction(Operations::CALL, hello_world.func);


        program.append_instruction(Operations::MOV, Register::A, Value(1234));
        program.append_instruction(Operations::CALL, printer.func);
        program.append_instruction(Operations::OUT, Value('\n'));

        program.append_instruction(Operations::PUSH, Value(50));
        program.append_instruction(Operations::PUSH, Value(250));
        program.append_instruction(Operations::PUSH, Value(75));
        program.append_instruction(Operations::CALL, lerp.func);
        program.append_instruction(Operations::CALL, printer.func);
        program.append_instruction(Operations::OUT, Value('\n'));
        program.append_instruction(Operations::OUT, Value('\n'));
*/
        program.append_instruction(Operations::MOV, Register::A, Register::SP);

        const ui32 array_size = 50;
        const ui32 value_max = 1000;

        for(ui32 i = 0; i < array_size; ++i)
            program.append_instruction(Operations::PUSH, Value(rand() % value_max));

        program.append_instruction(Operations::OUT, Value('['));
        program.append_instruction(Operations::DBG, Displacement(Register::A, 0));
        for(ui32 i = 1; i < array_size; i++) {
            program.append_instruction(Operations::OUT, Value(','));
            program.append_instruction(Operations::OUT, Value(' '));
            program.append_instruction(Operations::DBG, Displacement(Register::A, i*4));
        }
        program.append_instruction(Operations::OUT, Value(']'));
        program.append_instruction(Operations::OUT, Value('\n'));

        program.append_instruction(Operations::MOV, Register::B, Value(array_size));
        program.append_instruction(Operations::CALL, sort.func);

        program.append_instruction(Operations::OUT, Value('['));
        program.append_instruction(Operations::DBG, Displacement(Register::A, 0));
        for(ui32 i = 1; i < array_size; i++) {
            program.append_instruction(Operations::OUT, Value(','));
            program.append_instruction(Operations::OUT, Value(' '));
            program.append_instruction(Operations::DBG, Displacement(Register::A, i*4));
        }
        program.append_instruction(Operations::OUT, Value(']'));
        program.append_instruction(Operations::OUT, Value('\n'));


        program.append_instruction(Operations::HLT);

        program.verify_labels();
        sandbox.load_program(program);
    }

    auto chrono_start = std::chrono::high_resolution_clock::now();
    std::cout << "# Start #" << std::endl;

    while(!sandbox.is_halted()) {
        //auto is = sandbox.get_instruction();
        //std::cout << "\033[31m\033[1m< " << sandbox.get_pc() << " : " << is.to_string() << " >\033[0m"; 
        Interpreter::run_next_instruction(sandbox);
        //std::cout << '\n';
        //std::cin.get();
    }

    std::cout << "# Halt #" << std::endl;
    auto elapsed = std::chrono::high_resolution_clock::now() - chrono_start;
    auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    std::cout << "Duration : " << nanos / 1'000'000 << " ms (" << nanos / 1'000'000'000. << " s)\n";

}