# TODO Start: [Student] Complete Dockerfile
FROM node:20 AS builder
WORKDIR /builder
RUN if ! [ -f /usr/local/bin/yarn ]; then npm install yarn; fi
COPY package.json yarn.lock ./
RUN yarn config set registry https://registry.npmmirror.com
RUN yarn install
COPY . .
RUN yarn next build
FROM node:20 AS runner
ENV PORT=80
WORKDIR /app
COPY --from=builder /builder/.next/standalone ./standalone
COPY --from=builder /builder/public ./standalone/public
COPY --from=builder /builder/.next/static ./standalone/.next/static
EXPOSE 80
CMD [ "node", "standalone/server.js" ]
# TODO End