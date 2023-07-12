import { StatusBar } from 'expo-status-bar';
import React from 'react';
import { StyleSheet, Button, View } from 'react-native';

export default function App() {
  return (
    <View style={styles.container}>
      <View style={styles.directionButtons}>
        <Button title='north' />
        <View style={styles.sideButtons}>
          <Button title='west' />
          <Button title='east' />
        </View>
        <Button title='south' />
      </View>
      <View style={styles.bottomButtons}>
        <Button title='Btn_01' />
        <Button title='Btn_02' />
      </View>
      <StatusBar style="auto" />
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#000',
    alignItems: 'center',
    justifyContent: 'center',
  },
  directionButtons: {
    flexDirection: 'column',
    alignItems: 'center',
    gap: 70
  },
  sideButtons: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    gap: 150
  },
  bottomButtons: {
    flexDirection: 'row',
    position: 'absolute',
    bottom: 100,
    marginBottom: 20,
    gap: 100
  },
});
