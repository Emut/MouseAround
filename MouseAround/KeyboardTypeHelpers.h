#pragma once

bool WinToArduino(unsigned char& par_ucKey) {
	if (par_ucKey >= 'A' && par_ucKey <= 'Z') {
		return true;
	}
	return false;
}
