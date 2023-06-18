# Just a simple struct to store data on the color/ratio range
class ColRange(): 
    def __init__(self, lower=[ -5, -100, -100 ], upper=[ 5, 100, 100 ], lower_ratio=3.0/5.0, upper_ratio=5.0/3.0):
        self.lower_h_range = lower[0]
        self.lower_s_range = lower[1]
        self.lower_v_range = lower[2]

        self.upper_h_range = upper[0]
        self.upper_s_range = upper[1]
        self.upper_v_range = upper[2]

        self.lower_ratio = lower_ratio
        self.upper_ratio = upper_ratio
