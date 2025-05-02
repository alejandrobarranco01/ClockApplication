LIBRARY ieee;
USE ieee.std_logic_1164.ALL;

ENTITY bcd_7segment IS
    PORT (
        bcd_in : IN STD_LOGIC_VECTOR (3 DOWNTO 0);
        seven_segment_out : OUT STD_LOGIC_VECTOR (6 DOWNTO 0));
END bcd_7segment;

ARCHITECTURE logic OF bcd_7segment IS
BEGIN
    PROCESS (bcd_in)
        VARIABLE A : STD_LOGIC := bcd_in(3);
        VARIABLE B : STD_LOGIC := bcd_in(2);
        VARIABLE C : STD_LOGIC := bcd_in(1);
        VARIABLE D : STD_LOGIC := bcd_in(0);
    BEGIN
        seven_segment_out(0) <= NOT ((NOT A AND C) OR (NOT A AND B AND D) OR (A AND NOT B AND NOT C) OR (NOT A AND NOT B AND NOT D));
        seven_segment_out(1) <= NOT ((NOT A AND NOT B) OR (NOT A AND NOT C AND NOT D) OR (NOT A AND C AND D) OR (NOT B AND NOT C));
        seven_segment_out(2) <= NOT ((NOT A AND D) OR (NOT A AND B) OR (NOT B AND NOT C));
        seven_segment_out(3) <= NOT ((NOT A AND NOT B AND C) OR (NOT A AND B AND NOT C AND D) OR (NOT A AND C AND NOT D) OR (A AND NOT B AND NOT C) OR (NOT A AND NOT B AND NOT D));
        seven_segment_out(4) <= NOT ((NOT A AND C AND NOT D) OR (NOT B AND NOT C AND NOT D));
        seven_segment_out(5) <= NOT ((NOT A AND B AND NOT C) OR (NOT A AND B AND NOT D) OR (A AND NOT B AND NOT C) OR (NOT A AND NOT C AND NOT D));
        seven_segment_out(6) <= NOT ((NOT A AND NOT B AND C) OR (NOT A AND B AND NOT C) OR (NOT A AND C AND NOT D));
    END PROCESS;
END logic;