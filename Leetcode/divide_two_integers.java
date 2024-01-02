
class divide_two_integers{
	public static void main(String[] args){
		Solution s = new Solution();
		System.out.println(s.divide(7, -3));
	}
}

class Solution{
	public int divide(int dividend, int divisor){
		int count = 0;
		int sign = 1;
		if (dividend < 0) {
			sign = sign * -1;
			dividend = -dividend;
		}if (divisor < 0) {
			sign = sign * -1;
			divisor = -divisor;
		}while (dividend > 0){
			dividend = dividend - divisor;
			count++;
		}return (count - 1)*sign;
	}
}

