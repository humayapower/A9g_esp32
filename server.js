const express = require('express');
const { MongoClient } = require('mongodb');
const app = express();
const uri = "mongodb+srv://bijghavaneanjal23:LemXQf2fHs33SJB1@cluster0.drxvtlc.mongodb.net/?retryWrites=true&w=majority&appName=Cluster0";

// Middleware to parse JSON
app.use(express.json());

// CORS headers (required for ESP32)
app.use((req, res, next) => {
  res.header("Access-Control-Allow-Origin", "*");
  res.header("Access-Control-Allow-Headers", "Content-Type");
  next();
});

// POST endpoint for ESP32 data
app.post('/data', async (req, res) => {
  const client = new MongoClient(uri);
  try {
    await client.connect();
    const db = client.db("iot_db");
    await db.collection("sensor_data").insertOne({
      ...req.body,
      timestamp: new Date()
    });
    res.status(200).send("Data saved to MongoDB!");
  } catch (err) {
    console.error(err);
    res.status(500).send("Error saving data");
  } finally {
    await client.close();
  }
});

// Start server
const PORT = process.env.PORT || 3000;
app.listen(PORT, () => console.log(`Server running on port ${PORT}`));
