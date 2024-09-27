import 'package:flutter/material.dart';
import 'package:flutter_blue/flutter_blue.dart';

void main() => runApp(MyApp());

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(title: Text('BLE Connection')),
        body: BLEScreen(),
      ),
    );
  }
}

class BLEScreen extends StatefulWidget {
  @override
  _BLEScreenState createState() => _BLEScreenState();
}

class _BLEScreenState extends State<BLEScreen> {
  FlutterBlue flutterBlue = FlutterBlue.instance;
  BluetoothDevice? connectedDevice;
  List<BluetoothService> services = [];

  @override
  void initState() {
    super.initState();
    scanForDevices();
  }

  void scanForDevices() {
    flutterBlue.startScan(timeout: Duration(seconds: 4));
    flutterBlue.scanResults.listen((results) {
      for (ScanResult r in results) {
        if (r.device.name == "ArduinoNano33") {
          flutterBlue.stopScan();
          connectToDevice(r.device);
          break;
        }
      }
    });
  }

  void connectToDevice(BluetoothDevice device) async {
    await device.connect();
    setState(() {
      connectedDevice = device;
    });

    services = await device.discoverServices();
    setState(() {});
  }

  void writeCharacteristic(BluetoothCharacteristic characteristic) async {
    await characteristic.write([0x01]); // Write a value to the characteristic
  }

  @override
  Widget build(BuildContext context) {
    return Column(
      children: [
        if (connectedDevice != null)
          Text('Connected to ${connectedDevice!.name}'),
        if (services.isNotEmpty)
          ...services.map((service) {
            return Column(
              children: service.characteristics.map((c) {
                return ListTile(
                  title: Text(c.uuid.toString()),
                  trailing: IconButton(
                    icon: Icon(Icons.send),
                    onPressed: () => writeCharacteristic(c),
                  ),
                );
              }).toList(),
            );
          }),
      ],
    );
  }
}
