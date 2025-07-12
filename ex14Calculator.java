import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
public class ex14Calculator extends JFrame implements ActionListener {
 private JTextField display; // Text field to display results
 private double firstNumber = 0;
 private double secondNumber = 0;
 private String operator = "";
 private boolean isOperatorPressed = false;
 public ex14Calculator() {
 setTitle("Simple Calculator");
 setSize(400, 400);
 setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
 // Set up the display
 display = new JTextField();
 display.setHorizontalAlignment(JTextField.RIGHT);
 display.setEditable(false);
 display.setFont(new Font("Arial", Font.BOLD, 20));
 add(display, BorderLayout.NORTH);
 // Set up the button panel with GridLayout
 JPanel buttonPanel = new JPanel();
 buttonPanel.setLayout(new GridLayout(4, 4, 5, 5));
 // Add buttons
 String[] buttons = {
 "7", "8", "9", "/",
 "4", "5", "6", "X",
 "1", "2", "3", "-",
 "C", "0", "=", "+"
 };
 for (String text : buttons) {
 JButton button = new JButton(text);
 button.setFont(new Font("Arial", Font.BOLD, 18));
 button.addActionListener(this);
 buttonPanel.add(button);
 }
 add(buttonPanel, BorderLayout.CENTER);
 }
 @Override
 public void actionPerformed(ActionEvent e) {
 String command = e.getActionCommand();
 try {
 if ("0123456789".contains(command)) {
 if (isOperatorPressed) {
 display.setText("");
isOperatorPressed = false;
 }
 display.setText(display.getText() + command);
 } else if ("/-*+".contains(command)) {
 firstNumber = Double.parseDouble(display.getText());
 operator = command;
 isOperatorPressed = true;
 } else if ("=".equals(command)) {
 secondNumber = Double.parseDouble(display.getText());
 double result = calculate(firstNumber, secondNumber, 
 operator);
 display.setText(String.valueOf(result));
 firstNumber = result; // Allows chaining calculations
 } else if ("C".equals(command)) {
 display.setText("");
 firstNumber = 0;
 secondNumber = 0;
 operator = "";
 }
 } catch (NumberFormatException ex) {
 display.setText("Error"); 
 } catch (ArithmeticException ex) {
 display.setText("Cannot divide by zero");
 } }
 private double calculate(double num1, double num2, String operator) 
 {
 switch (operator) {
 case "+":
 return num1 + num2;
 case "-":
 return num1 - num2;
 case "*":
 return num1 * num2;
 case "/":
 if (num2 == 0) {
 throw new ArithmeticException("Divide by zero");
 }
 return num1 / num2;
 default:
 return 0; }
 }
 public static void main(String[] args) {
 SwingUtilities.invokeLater(() -> {
 ex14Calculator calculator = new ex14Calculator();
 calculator.setVisible(true);
 });
 } }