// This file is part of DelftProto.
// 
// DelftProto is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
// 
// DelftProto is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License along
// with DelftProto.  If not, see <http://www.gnu.org/licenses/>.

/// \file
/// Provides the Machine class.

#ifndef __MACHINE_HPP
#define __MACHINE_HPP

#include <types.hpp>
#include <data.hpp>
#include <stack.hpp>
#include <state.hpp>
#include <script.hpp>
#include <neighbour.hpp>
#include <neighbourhood.hpp>
#include <instructions.hpp>
#include <machineextension.hpp>
#include <machineid.hpp>

/// A virtual machine.
/**
 * \note This class is \ref extending "extensible".
 */
class Machine : public MachineExtension {
	
	public:
		
		/// The ID of this Machine.
		MachineId id;
		
		/// The standard stack.
		Stack<Data> stack;
		
		/// The environment stack.
		Stack<Data> environment;
		
		/// The globals.
		Stack<Data> globals;
		
		/// The state variables.
		Array<State> state;
		
		/// The list of all the neighbours.
		NeighbourHood hood;
		
	protected:
		
		/// A pointer to the next instruction.
		Address instruction_pointer;
		
		/// Boolean indicating whether execution (installation or a single run) has finished.
		bool is_finished;
		
		/// The callback stack.
		/**
		 * \see call
		 * \see retn
		 */
		Stack<Instruction> callbacks;
		
		/// The current neighbour, when iterating through them.
		/**
		 * Used by the hood instructions when iterating through the hood to let the neighbour-instructions know which neighbour is currently being processed.
		 */
		NeighbourHood::iterator current_neighbour;
		
		friend void Instructions::DEF_VM(Machine &);
		friend class HoodInstructions;
		
	public:
		
		/// The constructor.
		Machine() : instruction_pointer(0), callbacks(1) { hood.add(id); }
		
		/// \name Execution control
		/// \{
			
			/// Start an installation script.
			/**
			 * \note This does not execute an the installation script, it only prepares it. Call step() while not finished() to execute it.
			 * 
			 * \param script A pointer to the installation script.
			 */
			inline void install(Int8 const * script) {
				is_finished = false;
				jump(Address(script));
				callbacks.push(0);
			}
			
			/// Start a single run.
			/**
			 * \note This does not execute an entire run, it only prepares it. Call step() while not finished() to execute it.
			 */
			inline void run() {
				is_finished = false;
				call(instruction_pointer);
				for(size_t i = 0; i < state.size(); i++){
					if (!state[i].is_executed) state[i].data.reset();
					state[i].is_executed = false;
				}
			}
			
			/// Execute the next instruction.
			/**
			 * \note Do not use this function when already finished().
			 */
			inline void step() {
				execute(instructions[nextInt8()]);
			}
			
			/// Check whether the running script (installation or a single run) has finished (true) or not (false).
			inline bool finished() {
				return callbacks.empty();
			}
			
		/// \}
		
		/// \name Control flow
		/// \{
			
			/// Jump to another address.
			/**
			 * \param address The address to jump to.
			 */
			inline void jump(Address address) {
				instruction_pointer = address;
			}
			
			/// Advance the instruction pointer.
			/**
			 * \param distance The number of bytes to skip.
			 */
			inline void skip(Size distance) {
				instruction_pointer += distance;
			}
			
			/// Call a function.
			/**
			 * \param address The address of the first instruction of the function.
			 * \param callback Called when the function returns.
			 */
			inline void call(Address address, Instruction callback = 0) {
				callbacks.push(callback);
				stack.push(instruction_pointer);
				jump(address);
			}
			
			/// Return from a function.
			/**
			 * If the corresponding call (if any) registered a callback, it will be called.
			 * 
			 * \see Instructions::RET
			 */
			inline void retn() {
				Instruction callback = callbacks.pop();
				Data result = stack.pop();
				jump(stack.popAddress());
				if (!callbacks.empty() || callback) stack.push(result);
				if (callback) callback(*this);
			}
			
			/// Get the current address (instruction pointer).
			inline Address currentAddress() const {
				return instruction_pointer;
			}
			
			/// Get the callback depth.
			/**
			 * Usually 1, but higher inside instructions such as \ref Instructions::INIT_FEEDBACK "INIT_FEEDBACK" or \ref Instructions::TUP_MAP "TUP_MAP".
			 */
			inline Counter depth() const {
				return callbacks.size();
			}
			
		/// \}
		
		/// \name Low level
		/// \{
			
			/// Read the next byte as an unsigned 8-bit integer.
			inline Int8 nextInt8() {
				return *instruction_pointer++;
			}
			
			/// Read the next two bytes as an unsigned 16-bit integer (big endian).
			inline Int16 nextInt16() {
				Int16 data  = static_cast<Int16>(nextInt8()) << 8;
				      data += nextInt8();
				return data;
			}
			
			/// Execute an instruction.
			/**
			 * \param instruction The Instruction to execute.
			 * 
			 * \see Instructions
			 */
			inline void execute(Instruction instruction) {
				instruction(*this);
			}
			
		/// \}
		
		/// \name Neighbour methods
		/// \{
			
			/// Get the current neighbour, when iterating through them.
			/**
			 * Used by the hood instructions when iterating through the hood to let the neighbour-instructions know which neighbour is currently being processed.
			 */
			inline Neighbour & currentNeighbour() {
				return *current_neighbour;
			}
			
			/// Get the Neighbour representing this machine.
			inline Neighbour & thisMachine() {
				return *hood.begin();
			}
			
		/// \}
};

#endif
