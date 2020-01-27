from numpy import zeros, linspace, sin, cos, pi

# Recieves:
#  numpy array matrix of float or int to be transformed
#  float or int treshold value for ignoring the values that less than treshold
#  float or int scale value that represents the fraction of angle in radians in one pixel
# Returns:
#  numpy array matrix of int with shape 
#   (2 * size of matrix diagonal , int(180*scale)) result of hough transform
def hough(matrix, treshold=0, scale=1):
    maxdist = int( (matrix.shape[0]**2 + matrix.shape[1]**2)**0.5 )
    thetas =  linspace(-pi/2, pi/2, int(180*scale))
    sintheta = sin(thetas)
    costheta = cos(thetas)
    accumulator = zeros((2*maxdist, int(180*scale)))
    for y in range(matrix.shape[0]):
        for x in range(matrix.shape[1]):
            if(matrix[y][x] > treshold):
                for t in range(thetas.shape[0]):
                    r = int(round(x*costheta[t] + y*sintheta[t]) )
                    accumulator[r+maxdist][t] += 1
    return accumulator