Please click here or open `Project Brief.pdf` for project brief.
## Design 
# ALU
	For this section I chosed to do all the operation in parallel and using mux return the desired output. I used the builtin circuits for most of the parts. This circuit takes inputs, and a 4 bit alu select which is comming from control unit. Alu will do all the operations in parallel on both the inputs and return the value which hasbeen selected by the ALU select.

# REGFILE 
	For this section I created an array of 32 registers.
	Than I used two MUXes and two DEMUXes to chose the correct D, Q, and RegEn for the desired register. This unit takes 6 inputs. rs1, rs2, and rd are each 5 bit, RegWEn and clk are 1 bit, and a 32 bit write_Data. 
	It writes the value of write_Data on whenever clk and RegWEn are high. 

# Imm-gen 
	This is one of the crucial circuits in the system. This circuit takes Imm select, which is comming from the control-circuit, and instruction. This circuit analyzes the opcode, func3, and fuc7 parts of the circuit as necessary, and decides what type of Immediate to generate.
# control_logic
	This is the unit that sends signal to all the muxes. I implement this unit using the ROM implementation. It has 3 inputs, a 32 bit instruction, 1 bit BrEq, and 1 bit, BrLt. I used the and, and or gates, using func3, func7, and opcode values of the instruction to genrate the PC_select values and the generate all other signals, like imm-sel, A-sel, B-Sele, and ALU_Sel, CSRWEN, CSRWWR, BrUN, MEMRW, and WBSel from the ROM using a priority queue. 
# Branch Comperator
	This is the simplest circuit, which values store in rs1, rs2, and BrUn as input, and compare the values of rs1, and rs2 and sends signal that which type of branching is possible with such values. 
# CS61CPU
	For this section I designed the data path as single cyceled. The splitter acts as a decoder which defcodes the instruction into the neccary parts. This enabled me to implement all the parts and test them easily. later once the CPU was working, I pipleined the CPU into two stages. Stage one is fetching the instructions, and stage 2 exeecuting the instruction previously stored in the buffer register for the instruction. Moteover, I used another regist to store the value of the current programm counter. I feed this to Mux ASelect and added four with this PC and feed it to the writeback mux. To hunddle the memory operations I used a set of muxes to chose what to return and what to write in case of lw, lh, lb, sw, sh, and sb.

## Advantages and disadvantages
	Since I used ROM to generate the signals, it is going to be costly and possibly slower than if we do it using gates. However, the debugging was easier and didnt need alot of efforts. I think this is a good way to create  the prototype, but not feasible for mass production. After, the protoype is made the circuit coul be easily modified to generate the proper signal using simple gates. 

## Best or Worst Bugg
	My control signal in the ROM was off by one value and it was not perfoming the LUI opeation properly and that took me 2 hours to figure out. Moreover, the instruction for the sb operation was not that clear in specs sheet therefore it also cost me alot of time. Although, writing tests were the most tideous part of the project, I really enjoyed. It was almost like solving puzzles. Tick the clock and observe that values.