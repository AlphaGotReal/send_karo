
class Solution{
	//bruteforce
	public boolean isPalindrome0(String s){
		String str = "";
		for (int r = 0; r < s.length(); r++){
			int chr = s.charAt(r);
			if (chr < 97 && !(chr >= 48 && chr <= 57)) chr += 32;
			if ((chr >= 97 && chr <= 122) || (chr >= 48 && chr <= 57)) str += (char)chr;
		}int n = str.length();
		for (int r = 0; r < n/2; r++){
			if (str.charAt(r) != str.charAt(n-r-1)) return false;
		}return true;
	}
	public boolean isPalindrome(String s){
		int right = s.length()-1;
		int left = 0;
		int r = 0, l = 0, ir = 0, il = 0;
		while (right > left){
			r = s.charAt(right);
			l = s.charAt(left);
			ir = !((r >= 97 && r <= 122) || (r >= 48 && r <= 57));
			il = !((r >= 97 && r <= 122) || (r >= 48 && r <= 57));
			right -= ir;
			left += il;
			if (ir + il != 0) continue;

			if ((r == l) || (r - l = 32) || (l - r = 32)){
				right --;
				left ++;
				continue;
			}return false;
		}return true;
	}

}

class Run{
	public static void main(String[] a){
		String s = "A man, a plan, a canal: Panama";
		Solution sol = new Solution();
		System.out.println(sol.isPalindrome(s));
	}
}

