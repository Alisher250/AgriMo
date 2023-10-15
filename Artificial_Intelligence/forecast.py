import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.metrics import mean_squared_error, r2_score
from sklearn.preprocessing import StandardScaler
import numpy as np
import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import layers
import matplotlib.pyplot as plt

df = pd.read_csv('combined_data.csv')

X = df[["Radiation","Pressure","WindDirection(Degrees)","Speed","N","P","K","temperature","humidity","ph","rainfall"]]
y = df[["Radiation","Pressure","WindDirection(Degrees)","Speed","N","P","K","temperature","humidity","ph","rainfall"]]
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)
scaler = StandardScaler()
X_train = scaler.fit_transform(X_train)
X_test = scaler.transform(X_test)

model = keras.Sequential([
    layers.Dense(64, activation='relu', input_shape=(X_train.shape[1],)),
    layers.Dense(64, activation='relu'),
    layers.Dense(11)
])
model.summary()

model.compile(optimizer='adam', loss='mean_squared_error')
history = model.fit(X_train, y_train, epochs=100, batch_size=32, validation_split=0.2)

y_pred = model.predict(X_test)
mse = mean_squared_error(y_test, y_pred)
r2 = r2_score(y_test, y_pred)
print("Mean Squared Error:", mse)
print("R-squared (R2) Score:", r2)

model.save("weather_new_ver_1")

keras.utils.plot_model(model, to_file="model_plot.png", show_shapes=True, show_dtype=True)

plt.imshow(plt.imread("model_plot.png"))
plt.axis('off')
plt.show()

plt.figure(figsize=(14, 6))

plt.subplot(1, 2, 1)
plt.plot(history.history['loss'], label='Training Loss')
plt.plot(history.history['val_loss'], label='Validation Loss')
plt.title('Training and Validation Loss')
plt.xlabel('Epoch')
plt.ylabel('Mean Squared Error')
plt.legend()

plt.subplot(1, 2, 2)
plt.scatter(y_test, y_pred, alpha=0.7)
plt.plot([y_test.min(), y_test.max()], [y_test.min(), y_test.max()], '--', lw=3, color='red')
plt.title('Actual vs. Predicted')
plt.xlabel('Actual Value')
plt.ylabel('Predicted Value')

plt.tight_layout()
plt.show()

def predict_weather(radiation, pressure, wind_direction, wind_speed, nitrogen, phosphorous, potassium, temperature, humidity, ph, rainfall):
    data = np.array([[radiation, pressure, wind_direction, wind_speed, nitrogen, phosphorous, potassium, temperature, humidity, ph, rainfall]])
    data = scaler.transform(data)
    predicted = model.predict(data)
    return predicted

predicted_data = predict_weather(1.21, 30.46, 176.78, 3.37, 85, 58, 41, 20.87974371, 82.00274423, 6.502985292000001, 202.9355362)
print("Predicted Values for New Data:")
print(predicted_data)

