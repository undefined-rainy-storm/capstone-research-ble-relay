import 'package:flutter/material.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import 'dart:async';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Bluetooth Scanner',
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.deepPurple),
        useMaterial3: true,
      ),
      home: const MyHomePage(title: 'Flutter Bluetooth Scanner'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({super.key, required this.title});

  final String title;

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  List<BluetoothDevice> scanResults = [];
  StreamSubscription? scanSubscription;

  Future<void> _scanForDevices() async {
    // Clear previous scan results
    setState(() {
      scanResults.clear();
    });

    // Start scanning
    FlutterBluePlus.startScan(timeout: const Duration(seconds: 4));

    // Listen to scan results
    scanSubscription = FlutterBluePlus.scanResults.listen((results) {
      setState(() {
        scanResults.clear();
        for (ScanResult each in results) {
          scanResults.add(each.device);
        }
      });
    });

    if (scanSubscription != null) {
      FlutterBluePlus.cancelWhenScanComplete(scanSubscription!);
    }

    // Stop scanning after 4 seconds
    /*
    await Future.delayed(const Duration(seconds: 4));
    await FlutterBluePlus.stopScan();
    */

    // Cancel the subscription
    /*
    await scanSubscription?.cancel();
    scanSubscription = null;
    */
  }

  @override
  void dispose() {
    scanSubscription?.cancel();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    // Use FlutterBluePlus.isScanning to get the scanning status
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: Text(widget.title),
      ),
      body: Center(
        child: StreamBuilder<bool>(
          stream: FlutterBluePlus.isScanning,
          initialData: false,
          builder: (context, snapshot) {
            final isScanning = snapshot.data ?? false;
            return Column(
              mainAxisAlignment: MainAxisAlignment.center,
              children: <Widget>[
                Text(
                  'Bluetooth ${scanResults.length} device${scanResults.length == 1 ? '' : 's'} found.',
                ),
                const SizedBox(height: 10),
                if (isScanning)
                  const CircularProgressIndicator()
                else
                  Expanded(
                    child: ListView.builder(
                      itemCount: scanResults.length,
                      itemBuilder: (context, index) {
                        return ListTile(
                          title: Text(
                              '${scanResults[index].advName} \n ${scanResults[index].remoteId}'),
                          onTap: () {
                            print(scanResults[index].remoteId);
                          },
                        );
                      },
                    ),
                  ),
                const SizedBox(height: 20),
                ElevatedButton(
                  onPressed: isScanning ? null : _scanForDevices,
                  child: Text(isScanning ? 'Scanning...' : 'Scan for Devices'),
                ),
              ],
            );
          },
        ),
      ),
    );
  }
}
