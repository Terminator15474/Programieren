import matplotlib.pyplot as plt

# define the cost function
def cost(x, y, w1, w2):
  return (w1 * x + w2 * y - 1) ** 2

# define the gradient descent function
def grad_descent(x, y, learning_rate, num_iterations):
  # initialize the parameters
  w1 = 0
  w2 = 0
  
  # initialize lists to store the values of w1 and w2 at each iteration
  w1_list = []
  w2_list = []
  
  # repeat the following steps for a specified number of iterations
  for i in range(num_iterations):
    # calculate the gradient of the cost function with respect to w1 and w2
    dw1 = 2 * (w1 * x + w2 * y - 1) * x
    dw2 = 2 * (w1 * x + w2 * y - 1) * y
    
    # update the parameters by subtracting the gradient times the learning rate
    w1 = w1 - learning_rate * dw1
    w2 = w2 - learning_rate * dw2
    
    # store the values of w1 and w2 at this iteration
    w1_list.append(w1)
    w2_list.append(w2)
  
  # return the final values of the parameters and the lists of w1 and w2 values
  return w1, w2, w1_list, w2_list

# test the gradient descent function
w1, w2, w1_list, w2_list = grad_descent(2, 3, 0.01, 100)
print(w1, w2) # should be close to 0.5, 0.5

# plot the values of w1 and w2 at each iteration
plt.plot(w1_list, w2_list)
plt.xlabel('w1')
plt.ylabel('w2')
plt.show()
