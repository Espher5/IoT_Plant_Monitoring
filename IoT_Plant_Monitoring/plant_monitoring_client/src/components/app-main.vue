<template> 
    <div id="chart-area">
        <sensor-chart  v-for="item in items" :key="item.topic" :topic="item.topic" :name="item.name"/>
    </div>  
</template>

<script>
import SensorChart from './sensor-chart.vue';

export default {
    components: { SensorChart },
    props: {
        items: {
            type: Array,
            default: null
        }
    }
}
</script>
