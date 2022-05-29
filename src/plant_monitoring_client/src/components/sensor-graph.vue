<template>
    <div class="sensor-graph">      
        <h2>Evolution of values for {{ name }}</h2>

        <apexchart
            :type="line"
            :options="chartOptions"
            :series="series">
        </apexchart>
    <button @click="updateChart">Update!</button>
    </div>
</template>


<script>
import axios from 'axios';
import VueApexCharts from "vue3-apexcharts";


export default {
    name: 'senor-graph',
    components: { 
        apexchart: VueApexCharts
    },

    props: {
        topic : {
            type: String,
            default: '/iot/water'
        },
        name : {
            type: String,
            default: 'data'
        },
    },

    data: function() {
        return {
            type: 'line',
            chartOptions: {
                chart: {
                    id: 'vuechart'
                },
            },
            series: [
                {
                    name:this.name,
                    data: [],
                },
            ],
            colors: [
                '#008FFB',
                '#00E396',
                '#FEB019',
                '#FF4560',
                '#775DD0'
            ]
        };
    },
    
    methods: {
        // Updates the MQTT data from the server every 10 seconds
        fetchData(topic) {
            this.fetchInterval = setInterval(() => {
                axios.get('http://localhost:3000/api?topic=' + topic).then(response => {
                    this.series[0].data = response.data;

                    console.log(response);
                }).catch(err => {
                    console.log(err);
                });
            }, 1000);            
        },
    },

    created() {
        this.fetchData(this.topic);
    },

    beforeUnmount() {
        clearInterval(this.fetchInterval);
    }
}
</script>


<style scoped>
.sensor-graph {
    margin-bottom: 300px;
}
</style>