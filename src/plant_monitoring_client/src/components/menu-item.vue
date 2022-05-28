<template>
    <button>{{label}}</button>
</template>

<script>
export default {
    props: {
        label: {
            type: String,
            default: 'Button'
        },
        topic: {
            type: String,
            default: ''
        }
    }
}
</script>