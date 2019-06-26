# Carry-Formula-In-Look-Ahead-Adder

This script is used to generate carry value for any number of bits in [Carry-lookahead adder](https://en.wikipedia.org/wiki/Carry-lookahead_adder):  

    Ci+1 = Gi + Pi*Ci  
  
By default, it generates 8 carries:  

    $ ./CarryFormulaInLookAheadAdder.py 8  
    C0 = C0  
    C1 = G0+C0*P0  
    C2 = G1+G0*P1+C0*P0*P1  
    C3 = G2+G1*P2+G0*P1*P2+C0*P0*P1*P2  
    C4 = G3+G2*P3+G1*P2*P3+G0*P1*P2*P3+C0*P0*P1*P2*P3  
    C5 = G4+G3*P4+G2*P3*P4+G1*P2*P3*P4+G0*P1*P2*P3*P4+C0*P0*P1*P2*P3*P4  
    C6 = G5+G4*P5+G3*P4*P5+G2*P3*P4*P5+G1*P2*P3*P4*P5+G0*P1*P2*P3*P4*P5+C0*P0*P1*P2*P3*P4*P5  
    C7 = G6+G5*P6+G4*P5*P6+G3*P4*P5*P6+G2*P3*P4*P5*P6+G1*P2*P3*P4*P5*P6+G0*P1*P2*P3*P4*P5*P6+C0*P0*P1*P2*P3*P4*P5*P6  
    C8 = G7+G6*P7+G5*P6*P7+G4*P5*P6*P7+G3*P4*P5*P6*P7+G2*P3*P4*P5*P6*P7+G1*P2*P3*P4*P5*P6*P7+G0*P1*P2*P3*P4*P5*P6*P7+C0*P0*P1*P2*P3*P4*P5*P6*P7  

And you can also pass argument:  

    $ ./CarryFormulaInLookAheadAdder.py 4
    C0 = C0
    C1 = G0+C0*P0
    C2 = G1+G0*P1+C0*P0*P1
    C3 = G2+G1*P2+G0*P1*P2+C0*P0*P1*P2
    C4 = G3+G2*P3+G1*P2*P3+G0*P1*P2*P3+C0*P0*P1*P2*P3
